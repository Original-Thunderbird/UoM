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
import org.apache.commons.math3.linear.AbstractRealMatrix;  //doc: http://commons.apache.org/proper/commons-math/javadocs/api-3.6/org/apache/commons/math3/linear/AbstractRealMatrix.html
import org.apache.commons.math3.linear.BlockRealMatrix; //doc: http://commons.apache.org/proper/commons-math/javadocs/api-3.6/org/apache/commons/math3/linear/BlockRealMatrix.html
import org.apache.commons.math3.linear.MatrixUtils; //doc: http://commons.apache.org/proper/commons-math/javadocs/api-3.6/org/apache/commons/math3/linear/MatrixUtils.html

final class Leader3
	extends PlayerImpl
{
	private int last_date = 0;
	private int simulation_time_span = 0; // for calculatinhttp://commons.apache.org/proper/commons-math/javadocs/api-3.6/org/apache/commons/math3/linear/MatrixUtils.htmlg profits, in the lab it's 30
	private double forgetting_factor = 1.07;
	private double[][] history_data;
	private double b, a; // u_l = [3*(b-a) - 30] / (6*b-20), derived from S_L = (u_l-1)(2-u_l+0.3*u_f) and uf = a+b*u_l
	//vars nneds to be tracked
	private BlockRealMatrix phi_x;
	private BlockRealMatrix p = new BlockRealMatrix(2,2);
	private BlockRealMatrix L = new BlockRealMatrix(2,1); //adjusting factor
	private BlockRealMatrix theta = new BlockRealMatrix(2,1); //parameter needs to be estimated
	//vars for storing intermediate result in updateEstimation(int p_date)
	private BlockRealMatrix p_inside_numerator;
	private double denominator, prediction_error;

	// initialisation
	private Leader3()
		throws RemoteException, NotBoundException
	{
		super(PlayerType.LEADER, "Leader3_RLS");
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
			updateEstimation(p_date);
		}

		m_platformStub.publishPrice(m_type, calOptimumLeaderPrice());
		last_date = p_date;
		System.out.println("Day " + p_date + ", a=" + a + ", b=" + b);
	}

	// Update estimation function
	// use Weighting Least Square with Forgetting Factor
	private void updateEstimation(int p_date)
			throws RemoteException
	{

		//use X and Y from yesterday to update theta for today
		double[][] vector_to_biuld_phi_x = {{1}, {m_platformStub.query(m_type, p_date-1).m_leaderPrice}};
		phi_x = new BlockRealMatrix(vector_to_biuld_phi_x);

		denominator = ((BlockRealMatrix)(phi_x.transpose())).multiply(p).multiply(phi_x).getData()[0][0]+forgetting_factor;

		p_inside_numerator = p.multiply(phi_x).multiply((BlockRealMatrix)phi_x.transpose()).multiply(p);
		p = (BlockRealMatrix)(p.subtract((BlockRealMatrix)(p_inside_numerator.scalarMultiply(1.0/denominator))).scalarMultiply(1.0/forgetting_factor));

		L = (BlockRealMatrix)(p.multiply(phi_x).scalarMultiply(1.0/denominator));

		prediction_error = m_platformStub.query(m_type, p_date-1).m_followerPrice - ((BlockRealMatrix)phi_x.transpose()).multiply(theta).getData()[0][0];
		theta = theta.add(L.scalarMultiply(prediction_error));

		double[][] vector_from_theta = theta.getData();
		a = vector_from_theta[0][0];
		b = vector_from_theta[1][0];
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
		double weight = 0.0;

		BlockRealMatrix p_add_factor, theta_add_factor;
		simulation_time_span = p_steps;

		for (int day = 1; day <= 100; day++) { 		// learn the previous 100 days' data
			weight = Math.pow(forgetting_factor, 100-day);
			double[][] vector_to_biuld_phi_x = {{1}, {m_platformStub.query(m_type, day).m_leaderPrice}};
			phi_x = new BlockRealMatrix(vector_to_biuld_phi_x);
			p_add_factor = (BlockRealMatrix)(phi_x.multiply((BlockRealMatrix)phi_x.transpose()).scalarMultiply(weight));
			theta_add_factor = (BlockRealMatrix)(phi_x.scalarMultiply(weight*m_platformStub.query(m_type, day).m_followerPrice));
			p = p.add(p_add_factor);
			theta = theta.add(theta_add_factor);
		}
		theta = ((BlockRealMatrix)(MatrixUtils.inverse(p))).multiply(theta);

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
		new Leader3();
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
