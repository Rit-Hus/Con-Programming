


class Bee extends Thread {
private SharedHoneyPot HoneyPot;

private volatile  boolean stop = false;
public Bee(SharedHoneyPot HoneyPot) {
    this.HoneyPot = HoneyPot;


}
// Method to signal the bee to stop producing honey
public void setStop(){
    this.stop = true;

}
// Method to check if the bee should stop producing honey
public void run() {

    int honey = 1;
while (!stop){

HoneyPot.addHoney(honey);


// Simulate time taken to produce honey
try {
  sleep (1000);
} catch (InterruptedException e) {}

}
}
}


class Bear extends Thread {
private SharedHoneyPot HoneyPot;

public Bear(SharedHoneyPot HoneyPot) {
    this.HoneyPot = HoneyPot;
}

// Method to consume honey from the pot
public void run() {
    while (true) {
        HoneyPot.consumeHoney();
        try {
            sleep(1000);
        } catch (InterruptedException e) {}
    }
}
}





/*
 * Fairness:
 * This solution is correct but not strictly fair. We use Java's intrinsic monitor
 * (synchronized) with wait()/notifyAll(). When notifyAll() is called, all waiting
 * threads wake up, but the JVM/OS scheduler decides which thread re-acquires the
 * monitor lock next. There is no FIFO guarantee, so a particular bee could in
 * theory be postponed repeatedly (starvation is possible, though unlikely in practice).
 */


public class SharedHoneyPot {

    // Shared resource: the honey pot
private int honey = 0;
private int capacity = 15;


 public synchronized  void addHoney(int amount){
     
// If the pot is full, wait until the bear consumes the honey
while(honey >= capacity){

    try {
        wait();
    } catch (InterruptedException e) {}



}
    honey += amount;
System.out.println(" Bee" + Thread.currentThread().getName() +"added " + amount + " units of honey. Total: " + honey);


// If the pot is full after adding honey, notify the bear to consume it
notifyAll();

}


// Method for the bear to consume honey from the pot
public synchronized void consumeHoney(){
    while(honey < capacity){
        try {
            wait();
        } catch (InterruptedException e) {}

    }
   
        honey = 0;
    notifyAll();
System.out.println("Honey consumed, remaining: " + honey);




  }


  }




