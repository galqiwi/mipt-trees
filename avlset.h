#include <memory>
#include <assert.h>
#include <string>


template<class T>
class avlset{
private:
    struct Node {
        T value;
        unsigned int height;
        Node* parent = nullptr;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

    public:
        template<class S>
        Node(S&& val) : value(std::forward<S>(val)), height(1) {}

        void update() {
            height = 0;
            if (left)
                height = left->height;
            if (right)
                height = std::max(height, right->height);
            height++;
        }
    };

    std::unique_ptr<Node> root;

private:
    void erase(Node* v);
    void rotate(Node* p, Node* v);
    void balance(Node* v);

public:
    string name() {
        return "avlset";
    }

    avlset() = default;
    bool find(const T& val) const;

    template<class S>
    bool insert(S&& val) {
        Node* cur = root.get();
        while (cur) {
            if (cur->value == val)
                return false;
            if (cur->value < val) {
                if (cur->right) {
                    cur = cur->right.get();
                    continue;
                }
                cur->right = std::make_unique<Node>(std::forward<S>(val));
                cur->right->parent = cur;
                cur->update();
                balance(cur);
                return true;
            }
            else {
                if (cur->left) {
                    cur = cur->left.get();
                    continue;
                }
                cur->left = std::make_unique<Node>(std::forward<S>(val));
                cur->left->parent = cur;
                cur->update();
                balance(cur);
                return true;
            }
        }
        root = std::make_unique<Node>(std::forward<S>(val));
        return true;
    }

    bool erase(const T& val);
};

template<class T>
bool avlset<T>::find(const T& val) const {
    Node* cur = root.get();
    while (cur) {
        if (cur->value == val)
            return true;
        cur = cur->value < val ? cur->right.get() : cur->left.get();
    }
    return false;
}

template<class T>
bool avlset<T>::erase(const T& val) {
    Node* cur = root.get();
    while (cur) {
        if (cur->value == val){
            erase(cur);
            return true;
        }
        cur = cur->value < val ? cur->right.get() : cur->left.get();
    }    
    return false;    
}

template<class T>
void avlset<T>::erase(Node* v){
    if (!v->left && !v->right){
        if (!v->parent) {
            root.reset();
            return;
        }
        Node* p = v->parent;
        if (p->left.get() == v)
            p->left.reset();                    
        else
            p->right.reset();
        balance(p);
        return;
    }
    if (v->left) {
        Node* cur = v->left.get();
        while (cur->right)
            cur = cur->right.get();
        std::swap(v->value, cur->value);
        erase(cur);
    } else {
        Node* cur = v->right.get();
        while (cur->left)
            cur = cur->left.get();
        std::swap(v->value, cur->value);
        erase(cur);
    }
}

template<class T>
void avlset<T>::rotate(Node* p, Node* v){
    v->parent = p->parent;

    std::unique_ptr<Node> clonev, clonep;
    bool isLeft;
    if (p->left.get() == v) {
        clonev.reset(p->left.release());
        isLeft = true;
    }
    else {
        clonev.reset(p->right.release());
        isLeft = false;
    }

    if (v->parent) {
        if (v->parent->left.get() == p) {
            clonep.reset(v->parent->left.release());
            v->parent->left.reset(clonev.release());
        }
        else {
            clonep.reset(v->parent->right.release());
            v->parent->right.reset(clonev.release());
        }
    } else {
        clonep.reset(root.release());
        root.reset(clonev.release());
    }

    if (isLeft){
        if (v->right)
            v->right->parent = p;
        p->left.reset(v->right.release());
        v->right.reset(clonep.release());
    } else {
        if (v->left)
            v->left->parent = p;
        p->right.reset(v->left.release());
        v->left.reset(clonep.release());
    }
    p->parent = v;
    p->update();
    v->update();
}

template<class T>
void avlset<T>::balance(Node* v) {
    if (!v)
        return;
    v->update();

    int leftHeight = 0;
    if (v->left)
        leftHeight = v->left->height;
    int rightHeight = 0;
    if (v->right)
        rightHeight = v->right->height;

    assert(abs(leftHeight - rightHeight) <= 2);
    if (abs(leftHeight - rightHeight) == 2) {
        if (rightHeight == leftHeight + 2){
            int rightH = 0;
            if (v->right->right)
                rightH = v->right->right->height;
            if (v->right->left && v->right->left->height > rightH)
                rotate(v->right.get(), v->right->left.get());
            rotate(v, v->right.get());
        }
        else {
            int leftH = 0;
            if (v->left->left)
                leftH = v->left->left->height;
            if (v->left->right && v->left->right->height > leftH)
                rotate(v->left.get(), v->left->right.get());
            rotate(v, v->left.get());
        }
    }
    balance(v->parent);    
}
