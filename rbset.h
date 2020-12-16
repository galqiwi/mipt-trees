#include <vector>
#include <iostream>
#include <assert.h>
#include <string>
#include <memory>

//окей, это не лучший мой код, но как есть

using namespace std;

#define RED true
#define BLACK false

template<class T>
class rbset {
private:
    struct Node;
public:
    rbset() = default;
    string name() {
        return "rbset";
    }
    void insert(T x) {
        if (root == NULL) {
            root = make_unique<Node>(x);
            root->getColor() = BLACK;
        } else {
            auto insert_to = find_node_(x);

            if (insert_to.first == 0) {
                insert_to.second->updateValue(x);
                return;
            }

            Node* new_node = new Node(x);
            if (insert_to.first == 1) {
                new_node->attach_left(insert_to.second);
            } else {
                new_node->attach_right(insert_to.second);
            }

            Node* repair_node = new_node;
            while (1) {
                Node* parent = repair_node->getParent();
                Node* uncle = repair_node->getUncle();
                Node* grandParent = repair_node->getGrandParent();

                if (parent == NULL) {
                    repair_node->getColor() = BLACK;
                    return;
                } else {
                    if (parent->getColor() == BLACK) {
                        return;
                    } else {
                        //you are red
                        //your parent is red
                        if (uncle != NULL && uncle->getColor() == RED) {
                            //your uncle is red
                            uncle->getColor() = BLACK;
                            parent->getColor() = BLACK;
                            grandParent->getColor() = RED;
                            repair_node = grandParent;
                        } else {
                            //your uncle is black
                            if (grandParent->getRight() == parent && repair_node == parent->getLeft()) {
                                parent->rotateRight();
                                repair_node = parent;
                                continue;
                            } else if (grandParent->getLeft() == parent && repair_node == parent->getRight()) {
                                parent->rotateLeft();
                                repair_node = parent;
                                continue;
                            }

                            Node* old_root = root.release();
                            if (repair_node == parent->getLeft()) {
                                grandParent->rotateRight();
                            } else {
                                grandParent->rotateLeft();
                            }
                            if (parent->getParent() == NULL) {
                                root.reset(parent);
                            } else {
                                root.reset(old_root);
                            }

                            parent->getColor() = BLACK;
                            grandParent->getColor() = RED;

                        }
                    }
                }
            }
        }
    }
    bool find(T val) {
        auto found = find_node_(val);
        return (found.first == 0);
    }


    void DeleteCase1(Node* n) {
        if (n->getParent() != NULL) {
            DeleteCase2(n);
        }
    }

    void DeleteCase2(Node* n) {
        Node* s = n->getSibling();

        if (s->getColor() == RED) {
            n->getParent()->getColor() = RED;
            s->getColor() = BLACK;
            if (n == n->getParent()->getLeft()) {
                if (n->getParent() == root.get()) {
                    root.release();
                    root.reset(n->getParent()->getRight());
                }
                n->getParent()->rotateLeft();
            } else {
                if (n->getParent() == root.get()) {
                    root.release();
                    root.reset(n->getParent()->getLeft());
                }
                n->getParent()->rotateRight();
            }
        }
        DeleteCase3(n);
    }

    void DeleteCase3(Node* n) {
        Node* s = n->getSibling();
        if ((n->getParent()->getColor() == BLACK) && (s->getColor() == BLACK) &&
                (s->getLeftColor() == BLACK) && (s->getRightColor() == BLACK)) {
            s->getColor() = RED;
            DeleteCase1(n->getParent());
        } else {
            DeleteCase4(n);
        }
    }

    void DeleteCase4(Node* n) {
        Node* s = n->getSibling();

        if ((n->getParent()->getColor() == RED) && (s->getColor() == BLACK) &&
                (s->getLeftColor() == BLACK) && (s->getRightColor() == BLACK)) {
            s->getColor() = RED;
            n->getParent()->getColor() = BLACK;
        } else {
            DeleteCase5(n);
        }
    }

    void DeleteCase5(Node* n) {
        Node* s = n->getSibling();
        if (s->getColor() == BLACK) {
            if ((n == n->getParent()->getLeft()) && (s->getRightColor() == BLACK) &&
                    (s->getLeftColor() == RED)) {
                s->getColor() = RED;
                s->getLeft()->getColor() = BLACK;
                s->rotateRight();
            } else if ((n == n->getParent()->getRight()) && (s->getLeftColor() == BLACK) &&
                                 (s->getRightColor() == RED)) {
                s->getColor() = RED;
                s->getRight()->getColor() = BLACK;
                s->rotateLeft();
            }
        }
        DeleteCase6(n);
    }

    void DeleteCase6(Node* n) {
        Node* s = n->getSibling();

        s->getColor() = n->getParent()->getColor();
        n->getParent()->getColor() = BLACK;

        if (n == n->getParent()->getLeft()) {
            s->getRight()->getColor() = BLACK;
            
            Node* new_root = NULL;
            if (n->getParent() == root.get()) {
                root.release();
                new_root = n->getSibling();
            }
            n->getParent()->rotateLeft();
            if (new_root != NULL) {
                root.reset(new_root);
            }
        } else {
            s->getLeft()->getColor() = BLACK;
            n->getParent()->rotateRight();
        }
    }


    void erase(T val) {
        auto found = find_node_(val);
        if (found.first != 0) {
            return;
        }
        Node* todel = found.second;

        if (todel->nChildren() == 2) {
            Node* new_todel = todel->getRight();
            while (new_todel->getLeft() != NULL) {
                new_todel = new_todel->getLeft();
            }
            todel->getValue() = new_todel->getValue();
            todel = new_todel;
        }

        if (todel->getColor()) {
            if (todel == todel->getParent()->getLeft()) {
                todel->getParent()->deleteLeft();
            } else {
                todel->getParent()->deleteRight();
            }
            
            if (todel->getLeft() != NULL) {
                Node* todel_left = todel->getLeft();
                todel->deleteLeft();
                todel_left->attach_left(todel->getParent());
            }
            if (todel->getRight() != NULL) {
                Node* todel_right = todel->getRight();
                todel->deleteRight();
                todel_right->attach_left(todel->getParent());
            }

            delete todel;
            return;
        }
        bool todel_is_null = true;

        if (todel->getLeft() != NULL) {
            todel_is_null = false;
            if (root.get() == todel) {
                delete root.release();
                root.reset(todel->getLeft());
                root.get()->getColor() = BLACK;
                root.get()->deleteParent();
                return;
            } else {
                Node* todel_left = todel->getLeft();
                todel->deleteLeft();

                if (todel == todel->getParent()->getLeft()) {
                    todel->getParent()->deleteLeft();
                    todel_left->attach_left(todel->getParent());
                } else {
                    todel->getParent()->deleteRight();
                    todel_left->attach_right(todel->getParent());
                }
                todel = todel_left;
            }
        } else {
            if (todel->getRight() != NULL) {
                todel_is_null = false;
                if (root.get() == todel) {
                    Node* new_root = todel->getRight();
                    todel->deleteRight();
                    delete root.release();
                    root.reset(new_root);
                    root.get()->getColor() = BLACK;
                root.get()->deleteParent();
                    return;
                } else {
                    Node* todel_right = todel->getRight();
                    todel->deleteRight();

                    if (todel == todel->getParent()->getLeft()) {
                        todel->getParent()->deleteLeft();
                        todel_right->attach_left(todel->getParent());
                    } else {
                        todel->getParent()->deleteRight();
                        todel_right->attach_right(todel->getParent());
                    }
                    delete todel;
                    todel = todel_right;
                }
            }        
        }
        if (todel->getColor()) {
            todel->getColor() = BLACK;
            return;
        }

        DeleteCase1(todel);

        if (todel_is_null) {
            if (todel->getParent() != NULL) {
                if (todel->getParent()->getLeft() == todel) {
                    delete todel->getParent()->deleteLeft();
                } else {
                    delete todel->getParent()->deleteRight();
                }
            } else {
                root.reset();
            }
        }
    }

    void print(int layer = 0, Node* base = NULL) {
        if (root.get() == NULL)
            return;

        if (layer == 0)
            base = root.get();
        
        if (base == NULL)
            return;

        for (int i = 0; i < layer; i++) {
            cout << "\t";
        }
        if (base->getColor())
            cout << "R";
        else
            cout << "B";

        cout << base->getValue() << endl;

        print(layer + 1, base->getLeft());
        print(layer + 1, base->getRight());
    }

    bool is_valid() {
        if (root.get() == NULL) {
            return true;
        }
        if (root->getColor()) {
            return false;
        }
        auto out = is_valid_rec(root.get());
        return out.first;
    }

    pair<bool, int> is_valid_rec(Node* node) {
        //returns validational bool and amount of black height

        int n_left = 0, n_right = 0;

        if (node->getLeft() != NULL) {
            auto tmp = is_valid_rec(node->getLeft());
            n_left = tmp.second;
            if (!tmp.first || node->getLeft()->getValue() > node->getValue() || node->getColor() && node->getLeftColor()) {
                return make_pair(false, -1);
            }
        } else {
            n_left = 1;
        }

        if (node->getRight() != NULL) {
            auto tmp = is_valid_rec(node->getRight());
            n_right = tmp.second;
            if (!tmp.first || node->getRight()->getValue() < node->getValue() || (node->getColor() && node->getRightColor())) {
                return make_pair(false, -1);
            }
        } else {
            n_right = 1;
        }

        if (n_right != n_left) {
            return make_pair(false, -1);
        }

        if (!node->getColor()) {
            n_right++;
        }
        return make_pair(true, n_right);
    }
private:
    unique_ptr<Node> root;
    pair<int, Node*> find_node_(T val) {
        // rerurns (0, pointer to node) if found,
        // (1, pointer to possible parent node) if not found and (val < parent->val)
        // (2, pointer to possible parent node) if not found and (val > parent->val)
        // (3, NULL) if tree is empty

        Node* lookup_node = root.get();

        if (lookup_node == NULL) {
            return make_pair(3, (Node*)NULL);
        }

        while (true) {
            if (val < lookup_node->getValue()) {
                
                if (lookup_node->getLeft() == NULL) {
                    return make_pair(1, lookup_node);
                } else {
                    lookup_node = lookup_node->getLeft();
                }

            } else {

                if (lookup_node->getValue() == val) {
                    return make_pair(0, lookup_node);
                }

                if (lookup_node->getRight() == NULL) {
                    return make_pair(2, lookup_node);
                } else {
                    lookup_node = lookup_node->getRight();
                }

            }
        }
    }

    struct Node {
        Node(T val) {
            val_ = val;
            colored = true;
            parent_ = NULL;
        }
        void attach_left(Node* parent) {
            parent->left_.release();
            parent->left_.reset(this);
            parent_ = parent;
        }
        void attach_right(Node* parent) {
            parent->right_.release();
            parent->right_.reset(this);
            parent_ = parent;
        }
        void rotateLeft() {
            assert(right_ != NULL);

            Node* new_root = right_.release();
            Node* parent = parent_;

            if (new_root->left_.get() != NULL) {
                new_root->left_->attach_right(this);
            } else {
            }

            if (parent != NULL) {
                if (this == parent->left_.get())
                    new_root->attach_left(parent);
                else
                    new_root->attach_right(parent);
            } else {
                new_root->parent_ = NULL;
            }

            attach_left(new_root);
        }
        void rotateRight() {
            assert(left_ != NULL);

            Node* new_root = left_.release();
            Node* parent = parent_;

            if (new_root->right_ != NULL) {
                new_root->right_->attach_right(this);
            } else {
            }

            if (parent != NULL) {
                if (this == parent->left_.get())
                    new_root->attach_left(parent);
                else
                    new_root->attach_right(parent);

            } else {
                new_root->parent_ = NULL;
            }

            attach_right(new_root);
        }
        int nChildren() {
            int out = 0;
            if (getLeft() != NULL) {
                out++;
            }
            if (getRight() != NULL) {
                out++;
            }
            return out;
        }
        Node* getParent() {
            return parent_;
        }
        Node* getGrandParent() {
            if (parent_ == NULL)
                return NULL;
            return parent_->parent_;
        }
        bool getLeftColor() {
            if (getLeft() == NULL) {
                return false;
            }
            return getLeft()->getColor();
        }
        bool getRightColor() {
            if (getRight() == NULL) {
                return false;
            }
            return getRight()->getColor();
        }
        void updateValue(T new_val) {
            val_ = new_val;
        }
        void deleteParent() {
            parent_ = NULL;
        }
        Node* getSibling() {
            if (parent_ == NULL)
                return NULL;
            if (parent_->getLeft() == this) {
                return parent_->getRight();
            } else {
                return parent_->getLeft();
            }
        }
        Node* getUncle() {
            if (parent_ == NULL)
                return NULL;
            return parent_->getSibling();
        }
        Node* deleteLeft() {
            return left_.release();
        }
        Node* deleteRight() {
            return right_.release();
        }
        Node* getLeft() {
            return left_.get();
        }
        Node* getRight() {
            return right_.get();
        }
        T& getValue() {
            return val_;
        }

        bool& getColor() {
            return colored;
        }
    private:
        T val_;
        Node *parent_;
        unique_ptr<Node> left_, right_;
        bool colored;
    };
};