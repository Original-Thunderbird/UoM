import comp34120.ex2.PlayerImpl;
import comp34120.ex2.PlayerType;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.util.Random;
import java.util.Timer;
import java.util.TimerTask;
/**
 * A very simple leader implementation that only generates random prices
 * @author Xin
 */
final class SimpleLeaderCustom
	extends PlayerImpl
{
	/* The randomizer used to generate random price */
	private final Random m_randomizer = new Random(System.currentTimeMillis());
	private int last_date = 0;
	private int simulation_time_span = 0;

	private SimpleLeaderCustom()
		throws RemoteException, NotBoundException
	{
		super(PlayerType.LEADER, "Simple Leader");
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
		// System.out.println(p_date);
		// System.out.println(m_platformStub.query(m_type, p_date-2).m_followerPrice);
		m_platformStub.publishPrice(m_type, genPrice(1.8f, 0.05f));
		last_date = p_date;
	}

	/**
	 * Generate a random price based Gaussian distribution. The mean is p_mean,
	 * and the diversity is p_diversity
	 * @param p_mean The mean of the Gaussian distribution
	 * @param p_diversity The diversity of the Gaussian distribution
	 * @return The generated price
	 */
	private float genPrice(final float p_mean, final float p_diversity)
	{
		return (float) (p_mean + m_randomizer.nextGaussian() * p_diversity);
	}

	@Override
	public void startSimulation(int p_steps)
		throws RemoteException
	{
		// TODO: Learn previous data information
		simulation_time_span = p_steps;
		m_platformStub.log(m_type, "startSimulation(): Not supported yet.");
	}

	@Override
	public void endSimulation()
		throws RemoteException
	{
		float[] follower_prices_history = new float[simulation_time_span];
		float[] leader_prices_history = new float[simulation_time_span];
		double leader_profit = 0;
		float tmp_follower_price = 0;
		float tmp_leader_price = 0;
		double tmp_profit = 0;
		int tmp_date = 0;
		for (int i = 0; i < simulation_time_span; i++) {
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
	}

	public static void main(final String[] p_args)
		throws RemoteException, NotBoundException
	{
		new SimpleLeaderCustom();
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
