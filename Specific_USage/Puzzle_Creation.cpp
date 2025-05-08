#include <cstddef>
#include <iostream>
#include <random>
#include <vector>

class disjointSet {
private:
    size_t size;
    int *parent;

public:
    /**
     * @brief Construct a new disjoint Set object
     * 
     * @param s 
     */
    disjointSet(size_t s) : size(s) {
        parent = new int[size];

        // initiallize
        for (int i = 0; i < size; i++) {
            parent[i] = -1;
        }
    }

    /**
     * @brief Destroy the disjoint Set object
     * 
     */
    ~disjointSet() {
        delete parent;
    }

    /**
     * @brief Union two subtree, merged by scale
     * 
     * @param root1 
     * @param root2 
     */
    void Union(int root1, int root2) {
        if (root1 == root2) {
            return;
        }

        // !attention, these are all negative
        if (parent[root1] > parent[root2]) {
            parent[root2] += parent[root1];
            parent[root1] = root2;
        } else {
            parent[root1] += parent[root2];
            parent[root2] = root1;
        }
    }

    /**
     * @brief find the set which x lies in, using recursion
     * 
     * @param x 
     * @return int 
     */
    int Find(int x) {
        if (parent[x] < 0) {
            // x is the root node
            return x;
        }

        // optimize
        return (parent[x] = Find(parent[x]));
    }
};


int generate_integer(int size) {
    // Create a random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, size * size - 1);

    return dis(gen);
}

void createPuzzle(int size) {
    int chosen_position, near_position;
    disjointSet ds(size * size);


    while (ds.Find(0) != ds.Find(size * size - 1)) {
        // 0 represents the starting position, while size^2-1 represents the ending positon, we need to ensure it is \to the same set
        while (true) {
            // chose the chosen_position randomly
            chosen_position = generate_integer(size);
            int find_index = ds.Find(chosen_position);

            // check up
            near_position = chosen_position - size;
            if (near_position >= 0 && ds.Find(near_position) != find_index) {
                break;
            }

            // check left
            near_position = chosen_position - 1;
            if (chosen_position % size != 0 && ds.Find(near_position) != find_index) {
                break;
            }

            // check down
            near_position = chosen_position + size;
            if (near_position < size * size && ds.Find(near_position) != find_index) {
                break;
            }

            // check right
            near_position = chosen_position + 1;
            if (near_position % size != 0 && ds.Find(near_position) != find_index) {
                break;
            }
        }

        // we have chosen the two positions, then we need to erase the wall between them
        ds.Union(ds.Find(chosen_position), ds.Find(near_position));

        // debugging info
        std::cout << "<" << chosen_position << "," << near_position << ">, ";
    }
}

void createPuzzle_with_visualization(int size){
    int chosen_position, near_position;
    disjointSet ds(size * size);

    // Initialize the maze grid with walls
    std::vector<std::vector<char>> maze(size * 2 + 1, std::vector<char>(size * 2 + 1, '#'));

    // Create the cells in the maze
    for (int i = 1; i < size * 2; i += 2) {
        for (int j = 1; j < size * 2; j += 2) {
            maze[i][j] = ' ';
        }
    }


    while (ds.Find(0) != ds.Find(size * size - 1)) {
        // 0 represents the starting position, while size^2-1 represents the ending position
        while (true) {
            // Choose the chosen_position randomly
            chosen_position = generate_integer(size);
            int find_index = ds.Find(chosen_position);

            // Check up
            near_position = chosen_position - size;
            if (near_position >= 0 && ds.Find(near_position) != find_index) {
                // Remove the wall between chosen_position and near_position
                maze[2 * (chosen_position / size) - 1][2 * (chosen_position % size)] = ' ';
                break;
            }

            // Check left
            near_position = chosen_position - 1;
            if (chosen_position % size != 0 && ds.Find(near_position) != find_index) {
                // Remove the wall between chosen_position and near_position
                maze[2 * (chosen_position / size)][2 * (chosen_position % size) - 1] = ' ';
                break;
            }

            // Check down
            near_position = chosen_position + size;
            if (near_position < size * size && ds.Find(near_position) != find_index) {
                // Remove the wall between chosen_position and near_position
                maze[2 * (chosen_position / size) + 1][2 * (chosen_position % size)] = ' ';
                break;
            }

            // Check right
            near_position = chosen_position + 1;
            if (near_position % size != 0 && ds.Find(near_position) != find_index) {
                // Remove the wall between chosen_position and near_position
                maze[2 * (chosen_position / size)][2 * (chosen_position % size) + 1] = ' ';
                break;
            }
        }

        // Union the two positions
        ds.Union(ds.Find(chosen_position), ds.Find(near_position));

        // Debugging info
        std::cout << "<" << chosen_position << "," << near_position << "> \n";

        // Print the maze after each wall removal
        for (const auto& row : maze) {
            for (char cell : row) {
                std::cout << cell;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

int main() {
    int size = 10;
    createPuzzle_with_visualization(size);
    return 0;
}
