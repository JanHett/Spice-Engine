#ifndef SPICE_NODE
#define SPICE_NODE

class Node {
private:
    const string _type;
    const
public:
    string id;
    
    virtual Node() = 0;
    virtual Node(const string id) = 0;
    // Node(const vector<weak_pointer<Node>> inputs, const vector<shared_pointer<Node>> outputs);
}

#endif // SPICE_NODE
