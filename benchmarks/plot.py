import matplotlib.pyplot as plt
import numpy as np
import csv

def main():
    my_data = np.genfromtxt('benchmark1.csv', delimiter=',')
    print(my_data.shape)
    print(my_data[:,0].shape)
    
    plt.plot(my_data[:,0],my_data[:,1])
    plt.plot(my_data[:,0],my_data[:,2])
    plt.plot(my_data[:,0],my_data[:,3])
    plt.show()

if __name__ == "__main__":
    main()