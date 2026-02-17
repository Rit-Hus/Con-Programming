


class Bee extends Thread {
private SharedHoneyPot HoneyPot;

private volatile  boolean stop = false;
public Bee(SharedHoneyPot HoneyPot) {
    this.HoneyPot = HoneyPot;


}
public void setStop(){
    this.stop = true;

}

public void run() {

    int honey = 1;
while (!stop){

HoneyPot.addHoney(honey);



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

public void run() {
    while (true) {
        HoneyPot.consumeHoney();
        try {
            sleep(1000);
        } catch (InterruptedException e) {}
    }
}
}






  public class SharedHoneyPot {

private int honey = 0;
private int capacity = 15;


 public synchronized  void addHoney(int amount){
     

while(honey >= capacity){

    try {
        wait();
    } catch (InterruptedException e) {}



}
    honey += amount;
System.out.println(" Bee" + Thread.currentThread().getName() +"added " + amount + " units of honey. Total: " + honey);

notifyAll();

}

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




