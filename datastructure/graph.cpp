#include <iostream>
#include <vector>
#include <list>
#include <initializer_list>
#include <string>
#include <sstream>

using namespace std;


class Graph
{
private:
    int vertices_;
    int edges_;
    vector<list<int>> adj_;

public:
    Graph(int vertices) : vertices_(vertices), edges_(0), adj_(vertices) { }
    Graph(initializer_list<int> l)
    {
        int count = 0;
        for(auto iter = l.begin(); iter != l.end(); iter++)
        {
            if(count == 0)
            {
                edges_ = *iter;
            }
            else if(count == 1)
            {
                vertices_ = *iter;
            }
            else if(count % 2 == 0)
            {
                addEdge(*(iter-1), *iter);
            }
            count++;
        }
    }

    void addEdge(int v, int w)
    {
        adj_[v].push_front(w);
        adj_[w].push_front(v);
        edges_++;
    }

    int vertices() const { return vertices_; }
    int edges() const { return edges_; }

    const list<int>& adj(int vertex) const { return adj_[vertex]; }

    int degree(int vertex) const
    {
        return adj_[vertex].size();
    }

    int maxDegree() const 
    {
        int max = 0;
        for(size_t i = 0; i < adj_.size(); i++)
        {
            max = max > degree(i) ? max : degree(i);
        }
        return max;
    }

    double avgDegree() const
    {
        return 2 * edges() / vertices();
    }

    int numberOfSelfLoops() const 
    {
        int count = 0;
        for(size_t i = 0; i < adj_.size(); i++)
        {
            for(auto iter = adj_[i].begin(); iter != adj_[i].end(); iter++)
            {
                if(*iter == i)
                {
                    count++;
                }
            }
        }
        return count/2;
    }

    string toString() const
    {
        stringstream ss;
        ss << vertices() << " vertices, " << edges() << " edges\n";
        for(int v = 0; v < vertices(); v++)
        {
            ss << v << ": ";
            for(auto iter = adj_[v].begin(); iter != adj_[v].end(); iter++)
            {
                ss << *iter << " ";
            }
            ss << "\n";
        }
        return ss.str();
    }
};

int main()
{
    Graph g({13, 13,
             0, 5,
             4, 3,
             0, 1,
             9, 12,
             6, 4,
             5, 4,
             0, 2,
             11, 12,
             9, 10,
             0, 6,
             7, 8,
             9, 11,
             5, 3});
    cout << g.toString() << endl;
    return 0;
}