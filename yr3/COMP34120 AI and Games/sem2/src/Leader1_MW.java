import comp34120.ex2.PlayerImpl;
import comp34120.ex2.PlayerType;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.util.Random;
import java.util.Timer;
import java.util.TimerTask;
// import org.apache.commons.math4.stat.*; // import external packages for linear regression
import org.apache.commons.math3.stat.regression.*;


final class Leader1
	extends PlayerImpl
{
	private int last_date = 0;
	private int simulation_time_span = 0; // for calculating profits
	private SimpleRegression regression = new SimpleRegression(); // initialise linear regression class
	private final Random m_randomizer = new Random(System.currentTimeMillis()); //tmp
	private double[][] history_data;
	private double b, a; // ul = [3*(b-a) - 30] / (6*b-20), derived from SL = (ul-1)(2-ul+0.3uf) and uf = a+bul

	// initialisation
	private Leader1()
		throws RemoteException, NotBoundException
	{
		super(PlayerType.LEADER, "Leader1_MW");
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
		// Randomly publish price
		// m_platformStub.publishPrice(m_type, genPrice(1.8f, 0.05f)); // publish new price

		// use all history price for training
		// if (p_date > 101)
		// {
		// 	history_data[p_date-1-1][0] = m_platformStub.query(m_type, p_date-1).m_leaderPrice;
		// 	history_data[p_date-1-1][1] = m_platformStub.query(m_type, p_date-1).m_followerPrice;
		// 	regression.addData(history_data[p_date-1-1][0], history_data[p_date-1-1][1]); // import in newest data
		// 	updateEstimation();
		// }
		// m_platformStub.publishPrice(m_type, calOptimumLeaderPrice());
		// last_date = p_date;
		// System.out.println("a="+a+" b="+b);

		// use a moving window
		int window_size = 100, index = 0;
		regression = new SimpleRegression();
		double[][] data_in_window = new double[window_size+1][2];

		if (p_date > 101) // learn new data
		{
			history_data[p_date-1-1][0] = m_platformStub.query(m_type, p_date-1).m_leaderPrice;
			history_data[p_date-1-1][1] = m_platformStub.query(m_type, p_date-1).m_followerPrice;
		}

		for (int day_index = p_date-window_size-1; day_index < p_date; day_index++) 
		{
			data_in_window[index][0] = history_data[day_index][0];
			data_in_window[index][1] = history_data[day_index][1];
			index++;
		}
		regression.addData(data_in_window);
		updateEstimation();
		m_platformStub.publishPrice(m_type, calOptimumLeaderPrice());
		last_date = p_date;
		System.out.println("a="+a+" b="+b);
	}

	/**
	 * Generate a random price based Gaussian distribution. The mean is p_mean,
	 * and the diversity is p_diversity
	 * @param p_mean The mean of the Gaussian distribution
	 * @param p_diversity The diversity of the Gaussian distribution
	 * @return The generated price
	 */
	// private float genPrice(final float p_mean, final float p_diversity)
	// {
	// 	return (float) (p_mean + m_randomizer.nextGaussian() * p_diversity);
	// }

	// Update estimation function everyday
	private void updateEstimation() {
		b = regression.getSlope(); // update estimation
		a = regression.getIntercept();
	}

	private float calOptimumLeaderPrice()
	{
		return (float)((3*(b-a) - 30) / (6*b-20));
	}

	@Override
	public void startSimulation(int p_steps)
		throws RemoteException
	{
		simulation_time_span = p_steps;
		history_data = new double[100+p_steps][2];

		// learn the previous 100 days' data
		for (int day = 1; day <= 100; day++) {
			history_data[day-1][0] = m_platformStub.query(m_type, day).m_leaderPrice;
			history_data[day-1][1] = m_platformStub.query(m_type, day).m_followerPrice;
		}

		m_platformStub.log(m_type, "startSimulation(): Now Supported");

		// use all history price for training
		// regression.addData(history_data); // perform linear regression
		// b = regression.getSlope();
		// a = regression.getIntercept();


		// DEPRECATED, use only the first 100 days' data for training
		// only use 100 day data for deriving linear regression model
		// Estimated Follower's Reaction Function = a + b * ul(t), ul(t) is leader's price at day t
		// regression.addData(history_data); // perform linear regression
		// m_platformStub.log(m_type, "startSimulation(): Now Supported");
		// m_platformStub.log(m_type, "b=" + regression.getSlope());
		// m_platformStub.log(m_type, "a=" + regression.getIntercept());
		// m_platformStub.log(m_type, "StdErr=" + regression.getSlopeStdErr());
		// b = regression.getSlope();
		// a = regression.getIntercept();
	}

	@Override
	public void endSimulation()
		throws RemoteException
	{
		// Query and Store History Prices
		// float[] follower_prices_history = new float[simulation_time_span];
		// float[] leader_prices_history = new float[simulation_time_span];
		double leader_profit = 0;
		float tmp_follower_price = 0;
		float tmp_leader_price = 0;
		double tmp_profit = 0;
		int tmp_date = 0;
		for (int i = 0; i < simulation_time_span; i++) { // not include the first 100 days' profit
			tmp_date = last_date - simulation_time_span + i + 1;

			// Query and Store History Prices
			// follower_prices_history[i] = m_platformStub.query(m_type, tmp_date).m_followerPrice;
			// leader_prices_history[i] = m_platformStub.query(m_type, tmp_date).m_leaderPrice;
			
			// Calculate profits
			tmp_follower_price = m_platformStub.query(m_type, tmp_date).m_followerPrice;
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
		new Leader1();
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
