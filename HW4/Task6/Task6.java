
public class Task6 {
    

    public static void main(String[] args) {
SharedHoneyPot pot = new SharedHoneyPot();        
Bear bear = new Bear(pot);
Bee[] bees = new Bee[5];
for (int i = 0; i < bees.length; i++) {
    bees[i] = new Bee(pot);
}
for(Bee bee : bees){
    bee.start();
}
bear.start();
    }
}


