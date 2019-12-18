import matplotlib.pyplot as plt
import numpy as np

firework = np.array([5,50,100,150,200,250,300,350,400,450,500,550,600,800,1000,1200,1400,1600])
CPU_ori = np.array([6.8,28.7,57.1,84.1,99.9,100.0,100.0,100.0])
CPU_sav = np.array([6.0,22.4,41.2,59.5,77.1,96.5,100.0,100.0])
plt.figure(1)
plt.xlabel("# of fireworks (# of particles = # of fireworks * 30 * 24)")
plt.ylabel("CPU occupation rate(%)")
plt.plot(firework[:8], CPU_ori, label = 'switch off')
plt.plot(firework[:8], CPU_sav, label = 'switch on')
plt.legend()

FPS_ori = np.array([59,60,60,59,53,44,36,31,27,24,19,19,19,14,12,10,8,7])
FPS_sav = np.array([60,59,60,59,59,59,49,42,36,34,31,25,24,20,16,13,13,10])
plt.figure(2)
plt.xlabel("# of fireworks (# of particles = # of fireworks * 30 * 24)")
plt.ylabel("FPS")
plt.plot(firework, FPS_ori, label = 'switch off')
plt.plot(firework, FPS_sav, label = 'switch on')
plt.legend()
plt.show()