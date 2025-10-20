from typing import List, Optional

class BSTNode():
    def __init__(self, key=None):
        self.left = None
        self.right = None
        self.key = key


    def insert(self, key):

        if not self.key:
            self.key = key
            return
        
        if self.key == key:
            return

        if key < self.key:
            if self.left:
                self.left.insert(key)
                return
            self.left = BSTNode(key)
            return
        
        if self.right:
            self.right.insert(key)
            return
        self.right = BSTNode(key)

    def inorder(self,vals) -> List[str]:
        if self.key is None:
            return []
        
        if self.left:
            self.left.inorder(vals)

        vals.append(self.key)
        if self.right:
            self.right.inorder(vals)

        return vals