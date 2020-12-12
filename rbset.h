#include <vector>
#include <iostream>
#include <assert.h>
#include <string>
#include <memory>

using namespace std;

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
            root->getColor() = false;
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
                    repair_node->getColor() = false;
                    return;
                } else {
                    if (parent->getColor() == false) {
                        return;
                    } else {
                        //you are red
                        //your parent is red
                        if (uncle != NULL && uncle->getColor() == true) {
                            //your uncle is red
                            uncle->getColor() = false;
                            parent->getColor() = false;
                            grandParent->getColor() = true;
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

                            parent->getColor() = false;
                            grandParent->getColor() = true;

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
    void erase(T val) {
        auto found = find_node_(val);
        if (found.first == 1) {
            return;
        }
        Node* todel = found.second;
    }
private:
    unique_ptr<Node> root;
    pair<T, Node*> find_node_(T val) {
        // rerurns (0, pointer to node) if found,
        // (1, pointer to possible parent node) if not found and (val < parent->val)
        // (2, pointer to possible parent node) if not found and (val > parent->val)

        Node* lookup_node = root.get();
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
                // this->right_ = NULL;
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
                // this->left_ = NULL;
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
        Node* getParent() {
            return parent_;
        }
        Node* getGrandParent() {
            if (parent_ == NULL)
                return NULL;
            return parent_->parent_;
        }
        Node* updateValue(T new_val) {
            val_ = new_val;
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
        Node* getLeft() {
            return left_.get();
        }
        Node* getRight() {
            return right_.get();
        }
        T getValue() {
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