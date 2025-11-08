import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CountDownLatch;

public class TreeNode {
    TreeNode parent = null;
    List<TreeNode> children = new ArrayList<TreeNode>();
    String name; //pt constructor
    
    // constructor
    public TreeNode(String name) {
        this.name = name;
    }
    
    ///functie care acceseaza un nod si ii adauga un copil
    /// copilul intra in lista de children al nodului
    /// apoi nodul nostru este setat ca parinte pt acel copil
    public synchronized void addChild(TreeNode child) { 
        if(!this.children.contains(child)) {
            this.children.add(child);
            child.setParentOnly(this); //adauga copil pt nodul accesat
        }
    }
    
    public synchronized void addChildOnly(TreeNode child) {
        if(!this.children.contains(child)) { 
            this.children.add(child); 
        }
    }   
    
    ///functie care acceseaza un nod, si ii seteaza un parinte
    /// nodul apoi este in lista de children al parintelui
    public synchronized void setParent(TreeNode parent) {
        this.parent = parent;
        parent.addChildOnly(this); //seteaza un tata pt nodul accesat
    }
    
    public synchronized void setParentOnly(TreeNode parent) {
        this.parent = parent;
    }
    
    public static void exemplu_corect(){
        System.out.println("EXEMPLU CORECT");
        TreeNode radacina = new TreeNode("A");
        TreeNode copil1 = new TreeNode("B");
        TreeNode copil2 = new TreeNode("C");
        TreeNode copil_copil1 = new TreeNode("D");

        radacina.addChild(copil1);
        radacina.addChild(copil2);
        copil1.addChild(copil_copil1);

        System.out.println(radacina.name);
        for(int i = 1; i <= radacina.children.size(); i++){ ///luam copiii radacinei
            System.out.println("|_"+radacina.children.get(i-1).name);
            for(int j = 1; j <=radacina.children.get(i-1).children.size(); j++) ///luam copiii copiilor
                System.out.println("  |__"+ radacina.children.get(i-1).children.get(j-1).name);

        }
    }

    
    public static void exemplu_deadlock() throws InterruptedException {
    System.out.println("Exemplu deadlock");
    TreeNode A = new TreeNode("A");
    TreeNode B = new TreeNode("B");
    
    CountDownLatch latch = new CountDownLatch(2);
    
    Thread thread1 = new Thread(() -> {
        synchronized(A) {
            System.out.println("Thread 1: locked A");
            latch.countDown(); // 2 -> 1
            
            try { 
                latch.await(); // asteptam t2 sa faca si el operatia
            } catch (InterruptedException e) {}
            
            System.out.println("Thread 1: trying to lock B");
            A.addChild(B); 
        }
        System.out.println("Thread 1: SUCCESS (nu se va printa)");
    });
    
    Thread thread2 = new Thread(() -> {
        synchronized(B) {
            System.out.println("Thread 2: locked B");
            latch.countDown(); // 1 -> 0
            
            try { 
                latch.await(); // asteptam t1 sa acceseze countDown()
            } catch (InterruptedException e) {}
            
            System.out.println("Thread 2: trying to lock A");
            B.addChild(A); 
        }
        System.out.println("Thread 2: SUCCESS (nu se va printa)");
    });
    
    thread1.start();
    thread2.start();
    
    // Check if deadlock occurred
    thread1.join(3000);
    thread2.join(3000);
    
    if(thread1.isAlive() && thread2.isAlive()) {
        System.out.println("\ndeadlock confirmat, ambele threaduri sunt in viata si asteapta pt celalalt");
    }
}
  
    public static void main(String[] args) {
        

        exemplu_corect();
        try{
        exemplu_deadlock();
        }
        catch(InterruptedException e){}

    }
}