import java.util.ArrayList;
import java.util.List;

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

    public static void exemplu_deadlock(){

    }
  
    public static void main(String[] args) {
        
        exemplu_corect();
        //exemplu deadlock

    }
}