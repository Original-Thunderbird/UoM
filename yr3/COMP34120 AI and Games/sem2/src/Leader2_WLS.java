import comp34120.ex2.PlayerImpl;
import comp34120.ex2.PlayerType;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.util.Random;
import java.util.Timer;
import java.util.TimerTask;
import java.lang.Math;
import org.apache.commons.math3.stat.regression.*;
import org.apache.commons.math3.fitting.*;

final class Leader2
	extends PlayerImpl
{
	private int last_date = 0;
	private int simulation_time_span = 0; // for calculating profits, in the lab it's 30
	private WeightedObservedPoints obs;   // doc: https://commons.apache.org/proper/commons-math/javadocs/api-3.6/org/apache/commons/math3/fitting/WeightedObservedPoints.html
	private PolynomialCurveFitter fitter; // doc: https://commons.apache.org/proper/commons-math/javadocs/api-3.6/org/apache/commons/math3/fitting/PolynomialCurveFitter.html
	private double[] coeff;
	private double forgetting_factor = 1.07;
	private double[][] history_data;
	private double b, a; // u_l = [3*(b-a) - 30] / (6*b-20), derived from S_L = (u_l-1)(2-u_l+0.3*u_f) and uf = a+b*u_l

	// initialisation
	private Leader2()
		throws RemoteException, NotBoundException
	{
		super(PlayerType.LEADER, "Leader2_WLS");
	}

	@Override
	public void goodbye()
		throws RemoteException
	{
		ExitTask.exit(500);
	}

	/**
	 * To inform this instance to proceed to a new simulation day
	 * @param p_date The date of the new day
	 * @throws RemoteException
	 */
	@Override
	public void proceedNewDay(int p_date)
		throws RemoteException
	{
		if (p_date > 101) // learn new data
		{
			history_data[p_date-1-1][0] = m_platformStub.query(m_type, p_date-1).m_leaderPrice;
			history_data[p_date-1-1][1] = m_platformStub.query(m_type, p_date-1).m_followerPrice;
		}

		updateEstimation(p_date);
		m_platformStub.publishPrice(m_type, calOptimumLeaderPrice());
		last_date = p_date;
		System.out.println("Day " + p_date + ", a=" + a + ", b=" + b);
	}

	// Update estimation function
	// use Weighting Least Square with Forgetting Factor
	private void updateEstimation(int p_date) {
		double weight = 0.0;
		obs = new WeightedObservedPoints();
		fitter = PolynomialCurveFitter.create(1); // Instantiate a first-degree polynomial fitter.

		for (int day_index = 0; day_index < p_date; day_index++)
		{
			weight = Math.pow(forgetting_factor, p_date - (day_index + 1));
			obs.add(weight, history_data[day_index][0], history_data[day_index][1]); // weight, leader_price, follower_price
		}
		coeff = fitter.fit(obs.toList());
		a = coeff[0];
		b = coeff[1];
	}

	private float calOptimumLeaderPrice() {
		float u_l = (float)((3*(b-a) - 30) / (6*b-20));
		// check if the u_l that gets derivative=0 can get profit>=0
		if ( ((u_l-1)*(2-u_l+0.3*(a+b*u_l)) >= 0) && (u_l >= 1) )
			return u_l;
		else
			return 1;
	}

	@Override
	public void startSimulation(int p_steps)
		throws RemoteException
	{
		simulation_time_span = p_steps;
		history_data = new double[100+p_steps][2];
		for (int day = 1; day <= 100; day++) { 		// learn the previous 100 days' data
			history_data[day-1][0] = m_platformStub.query(m_type, day).m_leaderPrice;
			history_data[day-1][1] = m_platformStub.query(m_type, day).m_followerPrice;
		}
		m_platformStub.log(m_type, "startSimulation(): Now Supported");

	}

	@Override
	public void endSimulation()
		throws RemoteException
	{
		double leader_profit = 0;
		float tmp_follower_price = 0;
		float tmp_leader_price = 0;
		double tmp_profit = 0;
		int tmp_date = 0;
		for (int i = 0; i < simulation_time_span; i++) { // not include the first 100 days' profit
			tmp_date = last_date - simulation_time_span + i + 1;
			tmp_follower_price = m_platformStub.query(m_type, tmp_date).m_followerPrice; // Calculate profits
			tmp_leader_price = m_platformStub.query(m_type, tmp_date).m_leaderPrice;
			tmp_profit = (tmp_leader_price - 1) * (2 - tmp_leader_price + 0.3*tmp_follower_price);
			leader_profit += tmp_profit;
		}

		m_platformStub.log(m_type, String.format("The overall profit is %.2f", leader_profit));
		m_platformStub.log(m_type, "endSimulation(): Now supported.");
		System.out.println("******Simulation Ends******\n\n");
	}

	public static void main(final String[] p_args)
		throws RemoteException, NotBoundException
	{
		new Leader2();
	}

	/**
	 * The task used to automatically exit the leader process
	 * @author Xin
	 */
	private static class ExitTask
		extends TimerTask
	{
		static void exit(final long p_delay)
		{
			(new Timer()).schedule(new ExitTask(), p_delay);
		}
		
		@Override
		public void run()
		{
			System.exit(0);
		}
	}
}
