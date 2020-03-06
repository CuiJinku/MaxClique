#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cassert>
#include "mcqd.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "2 parameters needed" << endl;
        return -1;
    }
    int size = atoi(argv[1]);
    bool **policy = new bool *[size];
    bool **copy = new bool *[size];
    for (int i = 0; i < size; i++) {
        policy[i] = new bool[size];
        copy[i] = new bool[size];
    }

    ifstream file(argv[2]);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            file >> policy[i][j];
            copy[i][j] = policy[i][j];
        }
    }
    file.close();

    ofstream log("log.txt");
    log << " ";
    for (int i = 0; i < size; i++) {
        log << " " << (char)(i+'A');
    }
    log << endl;
    for (int i = 0; i < size; i++) {
        log << (char)(i+'A') << " ";
        for (int j = 0; j < size; j++) {
            log << copy[i][j] << " ";
        }
        log << endl;
    }
    log << endl;

    vector<vector<bool> > initial;

    int *qmax;
    int qsize;
    Maxclique m(copy, size);
    m.mcqdyn(qmax, qsize);
    while (qsize > 1) {

        // remove max clique
        vector<int> hub;
        for (int i = 0; i < qsize; i++) {
            int degree = -1;
            for (int j = 0; j < size; j++) {
                degree += copy[qmax[i]][j];
            }
            // std::cout << "Degree of " << (char)(qmax[i]+'A') << ": " << degree << endl;
            if (degree > qsize - 1) {
                hub.push_back(qmax[i]);
            }
        }

        for (int i = 0; i < qsize - 1; i++) {
            for (int j = i + 1; j < qsize; j++) {
                copy[qmax[i]][qmax[j]] = 0;
                copy[qmax[j]][qmax[i]] = 0;
            }
        }


        // std::cout << "Vertices which are incident with nodes not in clique" << endl;
        // std::cout << "hub size: " << hub.size() << endl;
        // for (int i = 0; i < hub.size(); i++) {
        //     std::cout << (char)(hub[i]+'A') << " ";
        // }
        // std::cout << endl;

        if (hub.size() > 1) {
            for (int i = 0; i < hub.size() - 1; i++) {
                for (int j = i + 1; j < hub.size(); j++) {
                    copy[hub[i]][hub[j]] = 1;
                    copy[hub[j]][hub[i]] = 1;
                }
            }
        }

        // std::cout << "qsize: " << qsize << endl;
        if(qsize > 1) {
            vector<bool> vlan(size, 0);
            for (int i = 0; i < qsize; i++) {
                vlan[qmax[i]] = 1;
            }
            // for (int i = 0; i < size; i++) {
            //     std::cout << vlan[i] << " "; 
            // }
            // std::cout << endl;
            initial.push_back(vlan);
        }
 

        // Log the process of removing max clique
        sort(qmax, qmax+qsize);
        log << "clique: " ;
        for (int i = 0; i < qsize; i++) {
            log << (char)(qmax[i]+'A') << " ";
        }
        log << endl << endl;

        log << " ";
        for (int i = 0; i < size; i++) {
            log << " " << (char)(i+'A');
        }
        log << endl;
        for (int i = 0; i < size; i++) {
            log << (char)(i+'A') << " ";
            for (int j = 0; j < size; j++) {
                log << copy[i][j] << " ";
            }
            log << endl;
        }
        log << endl;
        // Log end

        Maxclique m(copy, size);
        m.mcqdyn(qmax, qsize);
    }
    log.close();

    bool **configuration = new bool*[size];
    for (int i = 0; i < size; i++) {
        configuration[i] = new bool[initial.size()];
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < initial.size(); j++) {
            configuration[i][j] = initial[j][i];
            std::cout << configuration[i][j] << " ";
        }
        std::cout << endl;
    }
    std::cout << endl;

    bool **ap = new bool *[size];
    for (int i = 0; i < size; i++) {
        ap[i] = new bool[size];
    }


    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            ap[i][j] = false;
            // cout << ap[i][j] << " ";
        }
        // cout << endl;
    }
    // cout << endl;

    for(int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < initial.size(); k++) {
                ap[i][j] |= configuration[i][k] & configuration[j][k];
            }
        }
    }

    // for (int i = 0; i < size; i++) {
    //     for (int j = 0; j < size; j++) {
    //         cout << ap[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;

    // for (int i = 0; i < size; i++) {
    //     for (int j = 0; j < size; j++) {
    //         cout << policy[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            assert(ap[i][j] == policy[i][j]);
        }
    }

    // release the space
    for (int i = 0; i < size; i++) {
        delete[] ap[i];
    }
    delete[] ap;

    for (int i = 0; i < size; i++) {
        delete[] configuration[i];
    }
    delete[] configuration;

    for (int i = 0; i < size; i++) {
        delete[] copy[i];
    }
    delete[] copy;
    
    for (int i = 0; i < size; i++) {
        delete[] policy[i];
    }
    delete[] policy;

    std::cout << "Done" << endl;
    return 0;
}
