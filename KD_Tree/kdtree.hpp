#include <tuple>
#include <vector>
#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <iostream>


/**
 * An abstract template base of the KDTree class
 */
template<typename...>
class KDTree;

/**
 * A partial template specialization of the KDTree class
 * The time complexity of functions are based on n and k
 * n is the size of the KDTree
 * k is the number of dimensions
 * @typedef Key         key type
 * @typedef Value       value type
 * @typedef Data        key-value pair
 * @static  KeySize     k (number of dimensions)
 */
template<typename ValueType, typename... KeyTypes>
class KDTree<std::tuple<KeyTypes...>, ValueType> {
public:
    typedef std::tuple<KeyTypes...> Key;
    typedef ValueType Value;
    typedef std::pair<const Key, Value> Data;
    static inline constexpr size_t KeySize = std::tuple_size<Key>::value;
    static_assert(KeySize > 0, "Can not construct KDTree with zero dimension");

protected:
    struct Node {
        Data data;
        Node *parent;
        Node *left = nullptr;
        Node *right = nullptr;

        Node(const Key &key, const Value &value, Node *parent) : data(key, value), parent(parent) {}

        const Key &key() { return data.first; }

        Value &value() { return data.second; }
    };

public:
    /**
     * A bi-directional iterator for the KDTree
     * ! ONLY NEED TO MODIFY increment and decrement !
     */
    class Iterator {
    private:
        KDTree *tree;
        Node *node;

        // Constructor
        Iterator(KDTree *tree, Node *node) : tree(tree), node(node) {}

        /**
         * Increment the iterator
         * Time complexity: O(log n)
         * When a increment occurs, if the current node has a right
         * subtree, the next node should be the left most leaf in the right subtree; 
         * otherwise (if the current node doesn’t have a right subtree), 
         * you should move up (by parent pointer) and find the first ancestor node so that the current node is in
         * the left subtree of the ancestor node.
         * When you increment the right most leaf node in the tree, you’ll find that the
         * node is not in any of its ancestors’ left subtree, so you should end the loop and set the next node as nullptr.
         */
        void increment() {
            if(node->right){//if has right subtree, then increment will be the leftmost of the right subtree.
                node=node->right;
                while(node->left) node=node->left;
            }
            else{//go up and find a 'right' ancestor
                if (!node->parent) {//reach the root node
                    node= nullptr;
                    return;
                }
                //while(!node->parent->right || node->parent->right->data.first==node->data.first){
                while((node->parent->left && node->parent->left->data.first!=node->data.first) || !node->parent->left){
                    //if data==data, because left child must be smaller, so it must be a "left" parent
                    node=node->parent;
                    if (node->parent== nullptr) {
                        node= nullptr;
                        return;
                    }
                }
                node=node->parent;
            }
        }

        /**
         * Decrement the iterator
         * Time complexity: O(log n)
         * When a decrement occurs, if the current node has a left
         * subtree, the next node should be the right most leaf in the left subtree;
         * otherwise (if the current node doesn’t have a left subtree),
         * you should move up (by parent pointer) and find the first ancestor node so that the current node is in
         * the right subtree of the ancestor node.
         * When you decrement the left most leaf node(begin) in the tree, you’ll find that the
         * node is not in any of its ancestors’ right subtree, so you should throw an error.
         * If doing an decrement on the end iterator, set next node to the right most leaf node
         */
        void decrement() {
            // If doing an decrement on the end iterator, set next node to the right most leaf node
            if(node->left){
                node=node->left;
                while(node->right) node=node->right;
            }
            else{
                if(node->parent== nullptr){
                    node= nullptr;
                    return;
                }
                while((node->parent->right && node->parent->right->data.first!=node->data.first) || !node->parent->right) {
                    node = node->parent;
                    if (node->parent == nullptr) {
                        node = nullptr;
                        return;
                    }
                }
                node=node->parent;
            }
        }

    public:
        friend class KDTree;

        Iterator() = delete;

        Iterator(const Iterator &) = default;

        Iterator &operator=(const Iterator &) = default;

        Iterator &operator++() {
            increment();
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            increment();
            return temp;
        }

        Iterator &operator--() {
            decrement();
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp = *this;
            decrement();
            return temp;
        }

        bool operator==(const Iterator &that) const {
            return node == that.node;
        }

        bool operator!=(const Iterator &that) const {
            return node != that.node;
        }

        Data *operator->() {
            return &(node->data);
        }

        Data &operator*() {
            return node->data;
        }
    };

protected:                      // DO NOT USE private HERE!
    Node *root = nullptr;       // root of the tree
    size_t treeSize = 0;        // size of the tree

    /**
     * Find the node with key
     * Time Complexity: O(k log n)
     * @tparam DIM current dimension of node
     * @param key
     * @param node
     * @return the node with key, or nullptr if not found
     */
    template<size_t DIM>
    Node *find(const Key &key, Node *node) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if(!node) return nullptr;
        if(key == node->key()) return node;
        if(std::get<DIM>(key) < std::get<DIM>(node->key())) return find<DIM_NEXT>(key, node->left);
        else return find<DIM_NEXT>(key, node->right);
    }

    /**
     * Insert the key-value pair, if the key already exists, replace the value only
     * Time Complexity: O(k log n)
     * @tparam DIM current dimension of node
     * @param key
     * @param value
     * @param node
     * @param parent
     * @return whether insertion took place (return false if the key already exists)
     */
    template<size_t DIM>
    bool insert(const Key &key, const Value &value, Node *&node, Node *parent) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;

        // If the node is empty
        if(!node){
            node = new Node(key, value, parent);
            this->treeSize++;
            return true;
        }

        // If we find the node with the key
        if(key == node->key()){
            node->value() = value;
            return false;
        }

        // If not exist, insert the new pair
        if(std::get<DIM>(key) < std::get<DIM>(node->key())) insert<DIM_NEXT>(key, value, node->left, node);
        else insert<DIM_NEXT>(key, value, node->right, node);

        return true;
    }

    /**
     * Compare two keys on a dimension
     * Time Complexity: O(1)
     * @tparam DIM comparison dimension
     * @tparam Compare
     * @param a
     * @param b
     * @param compare
     * @return relationship of two keys on a dimension with the compare function
     */
    template<size_t DIM, typename Compare>
    static bool compareKey(const Key &a, const Key &b, Compare compare = Compare()) {
        if (std::get<DIM>(a) != std::get<DIM>(b)){
            return compare(std::get<DIM>(a), std::get<DIM>(b));
        }
        return compare(a, b);
    }

    /**
     * Compare two nodes on a dimension
     * Time Complexity: O(1)
     * @tparam DIM comparison dimension
     * @tparam Compare
     * @param a
     * @param b
     * @param compare
     * @return the minimum / maximum of two nodes
     */
    template<size_t DIM, typename Compare>
    static Node *compareNode(Node *a, Node *b, Compare compare = Compare()) {
        if (!a) return b;
        if (!b) return a;
        return compareKey<DIM, Compare>(a->key(), b->key(), compare) ? a : b;
    }

    /**
     * Find the minimum node on a dimension
     * Time Complexity: ?
     * @tparam DIM_CMP comparison dimension
     * @tparam DIM current dimension of node
     * @param node
     * @return the minimum node on a dimension
     */
    template<size_t DIM_CMP, size_t DIM>
    Node *findMin(Node *node) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;

        if(!node) return nullptr;
        Node* min = findMin<DIM_CMP, DIM_NEXT>(node->left);
        if(DIM_CMP != DIM){
            Node* rightMin = findMin<DIM_CMP, DIM_NEXT>(node->right);
            min = compareNode<DIM_CMP, std::less<>>(min, rightMin, std::less<>());
        }
        
        return compareNode<DIM_CMP, std::less<>>(min, node, std::less<>());
    }

    /**
     * Find the maximum node on a dimension
     * Time Complexity: ?
     * @tparam DIM_CMP comparison dimension
     * @tparam DIM current dimension of node
     * @param node
     * @return the maximum node on a dimension
     */
    template<size_t DIM_CMP, size_t DIM>
    Node *findMax(Node *node) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;

        if(!node) return nullptr;
        Node* max = findMax<DIM_CMP, DIM_NEXT>(node->right);
        if(DIM_CMP != DIM){
            Node* leftMax = findMax<DIM_CMP, DIM_NEXT>(node->left);
            max = compareNode<DIM_CMP, std::greater<>>(max, leftMax, std::greater<>());
        }

        return compareNode<DIM_CMP, std::greater<>>(max, node, std::greater<>());
    }

    template<size_t DIM>
    Node *findMinDynamic(size_t dim) {

        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize) {
            dim %= KeySize;
        }
        if (dim == DIM) return findMin<DIM, 0>(root);
        return findMinDynamic<DIM_NEXT>(dim);
    }

    template<size_t DIM>
    Node *findMaxDynamic(size_t dim) {

        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize) {
            dim %= KeySize;
        }
        if (dim == DIM) return findMax<DIM, 0>(root);
        return findMaxDynamic<DIM_NEXT>(dim);
    }

    /**
     * Erase a node with key (check the pseudocode in project description)
     * Time Complexity: max{O(k log n), O(findMin)}
     * @tparam DIM current dimension of node
     * @param node
     * @param key
     * @return nullptr if node is erased, else the (probably) replaced node
     */
    template<size_t DIM>
    Node *erase(Node *node, const Key &key) {
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if(!node) return nullptr;

        if(key == node->key()){
            if(!node->left && !node->right){
                delete node;
                this->treeSize--;
                return nullptr;
            }
            else if(node->right){ 
                // Find the minNode in the right subtree
                Node* minNode = this->findMin<DIM,DIM_NEXT>(node->right);

                // Replace current node with minNode
                node->value() = minNode->value();
                const_cast<Key&>(node->key()) = minNode->key();

                // Go recursively into current node's right subtree
                node->right = erase<DIM_NEXT>(node->right, node->key());
            }
            else if(node->left){
                // Find the maxNode in the left subtree
                Node* maxNode = this->findMax<DIM,DIM_NEXT>(node->left);

                // Replace current node with maxNode
                node->value() = maxNode->value();
                const_cast<Key&>(node->key()) = maxNode->key();

                // Go recursively into current node's left subtrees
                node->left = erase<DIM_NEXT>(node->left, node->key());
            }
        }
        else{
            if(std::get<DIM>(key) < std::get<DIM>(node->key())){
                node->left = erase<DIM_NEXT>(node->left, key);
            }
            else{
                node->right = erase<DIM_NEXT>(node->right, key);
            }
        }

        return node;
    }

    template<size_t DIM>
    Node *eraseDynamic(Node *node, size_t dim) {

        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
        if (dim >= KeySize) {
            dim %= KeySize;
        }
        if (dim == DIM) return erase<DIM>(node, node->key());
        return eraseDynamic<DIM_NEXT>(node, dim);
    }

    // Helper function
template<size_t DIM>
	static bool cmp(const std::pair<Key, Value>& s1, const std::pair<Key, Value>& s2) {
		return compareKey<DIM, std::less<>>(s1.first, s2.first);  // only sort on Key
	}

	static bool uniqueRule(const std::pair<Key, Value>& s1, const std::pair<Key, Value>& s2) {
		return s1.first == s2.first;
	}

	template<size_t DIM>
	void removeDuplicated(std::vector<std::pair<Key, Value>>& v) {
		stable_sort(v.begin(), v.end(), cmp<DIM>);
		auto last = unique(v.rbegin(), v.rend(), uniqueRule);
		v.erase(v.begin(), last.base());
	}


	template<size_t DIM>
	Node *TreeInit(std::vector<std::pair<Key, Value>> v, Node* parent)
	{
		constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;
		/*if (left > right)
		{
			return nullptr;
		}
		int mid = left + (right - left) / 2;
		std::nth_element(v.begin() + left, v.begin() + mid, v.begin() + right + 1, [](const std::pair<Key, Value>& lhs, const std::pair<Key, Value>& rhs) {
			return compareKey<DIM, std::less<>>(lhs.first, rhs.first);
			});
		auto median = v[mid];
		Node* node = new Node{ median.first, median.second, parent };
		treeSize++;
		node->left = TreeInit<DIM_NEXT>(v, left, mid - 1, node);
		node->right = TreeInit<DIM_NEXT>(v, mid + 1, right, node);
		return node;*/
		if (v.empty()) {
			return nullptr;
		}
		auto m = v.begin() + (v.size()) / 2;
		std::nth_element(v.begin(), m, v.end(), cmp<DIM>);
		Data newData = v.at((v.size()) / 2);
		Node* node;
		node = new Node(newData.first, newData.second, parent);
		node->parent = parent;
		std::vector<std::pair<Key, Value>> left(v.begin(), m);
		std::vector<std::pair<Key, Value>> right(m + 1, v.end());
		node->left = TreeInit<DIM_NEXT>(left, node);
		node->right = TreeInit<DIM_NEXT>(right, node);
		return node;
	}
    template<size_t DIM>
    Node* KDTree_helper(std::vector<std::pair<Key, Value>> v, Node* parent){
        if(v.empty()) return nullptr;
        constexpr size_t DIM_NEXT = (DIM + 1) % KeySize;

        // Find the medium, 
        size_t mid;
        if(v.size() % 2 == 0) mid = v.size()/2 - 1;
        else mid = v.size()/2;

        // Sort in this new dim
        for(size_t i = v.size() - 1; i > 0; i--){
            for(size_t j = 0; j < i; j++){
                if(compareKey<DIM, std::less<>> (v[j+1].first, v[j].first, std::less<>())){
                    std::swap(v[j], v[j+1]);
                }
            }
        }

        // Create 
        std::pair<Key, Value> v_medium(v[mid]);
        
        // Partition data
        std::vector<std::pair<Key, Value>> v_left(v.begin(), v.begin() + mid);
        std::vector<std::pair<Key, Value>> v_right(v.begin() + mid + 1, v.end());

        // Create a new node
        Node* n = new Node(v_medium.first, v_medium.second, parent);
        treeSize++;

        // Go recursively into left and right part
        n->left = KDTree_helper<DIM_NEXT>(v_left, n);
        n->right = KDTree_helper<DIM_NEXT>(v_right, n);
        return n;
    }

    Node* copy_node(Node* root, Node* parent_root){
        if(!root) return nullptr;

        Node* node_copy = new Node(root->key(), root->value(), parent_root);
        
        if(root->left) node_copy->left = copy_node(root->left, node_copy);
        if(root->right) node_copy->right = copy_node(root->right, node_copy);

        return node_copy;
    }

    void Destroy_helper(Node *root){
        if(root != nullptr){
            Destroy_helper(root->left);
            Destroy_helper(root->right);
            delete root;
        }
    }

public:
    KDTree() = default;

    /**
     * Time complexity: O(kn log n)
     * @param v we pass by value here because v need to be modified
     */
    explicit KDTree(std::vector<std::pair<Key, Value>> v) {
        // Sort
        if (!v.empty()) {
			removeDuplicated<0>(v);

			root = TreeInit<0>(v, nullptr);
			treeSize = v.size();
		}       
    }

    /**
     * Time complexity: O(n)
     */
    KDTree(const KDTree &that) {
        this->root = this->copy_node(that.root, nullptr);
        this->treeSize = that.treeSize;
    }

    /**
     * Time complexity: O(n)
     */
    KDTree &operator=(const KDTree &that) {
        Destroy_helper(this->root);
        this->root = this->copy_node(that.root, nullptr);
        this->treeSize = that.treeSize;
        return *this;
    }

    /**
     * Time complexity: O(n)
     */
    ~KDTree() {
        Destroy_helper(this->root);
    }

    Iterator begin() {
        if (!root) return end();
        auto node = root;
        while (node->left) node = node->left;
        return Iterator(this, node);
    }

    Iterator end() {
        return Iterator(this, nullptr);
    }

    Iterator find(const Key &key) {
        return Iterator(this, find<0>(key, root));
    }

    void insert(const Key &key, const Value &value) {
        insert<0>(key, value, root, nullptr);
    }

    template<size_t DIM>
    Iterator findMin() {
        return Iterator(this, findMin<DIM, 0>(root));
    }

    Iterator findMin(size_t dim) {
        return Iterator(this, findMinDynamic<0>(dim));
    }

    template<size_t DIM>
    Iterator findMax() {
        return Iterator(this, findMax<DIM, 0>(root));
    }

    Iterator findMax(size_t dim) {
        return Iterator(this, findMaxDynamic<0>(dim));
    }

    bool erase(const Key &key) {
        auto prevSize = treeSize;
        erase<0>(root, key);
        return prevSize > treeSize;
    }

    Iterator erase(Iterator it) {
        if (it == end()) return it;
        auto node = it.node;
        if (!it.node->left && !it.node->right) {
            it.node = it.node->parent;
        }
        size_t depth = 0;
        auto temp = node->parent;
        while (temp) {
            temp = temp->parent;
            ++depth;
        }
        eraseDynamic<0>(node, depth % KeySize);
        return it;
    }

    size_t size() const { return treeSize; }
};
