//
// Created by Martin on 06-02-2017.
//

#include <iostream>
#include <cstdlib>
#include "BinaryNode.h"
#include <time.h>
#include <windows.h>
#include <cmath>

using namespace std;

/*void buildPointerArray(int* parray, int* array, int x, int y, int start, int parent) {

	int middle = (y-x)/2 + x;

	parray[start] = array[middle];	// Value
	parray[start+1] = parent;		// Parent
	parray[start+2] = 2*start*4-3;		// Left
	parray[start+3] = (2*start)*4+1;	// Right

	cout << array[middle] << "," << parent << "," << 2*start*4 << "," << (2*start+1)*4 << '\n';

	if(y > x) {
		buildPointerArray(parray, array, x, middle-1, 2*start*4-3, start);
		buildPointerArray(parray, array, middle+1, y, (2*start)*4+1, start);
	}


}*/

int pos(int d, int i, int* array) {

    // i = BSF position, and d = depth
    // Pos[d] = Pos[D[d]] + T[d] + (i AND T[d]) * B[d]
    // array har form B[d] T[d] D[d] dvs. size af B, size af T og Ts dybde.

    if(d == 1) {
        return 1;
    }

    // Pos[D[d]
    int newI = i >> 1;
    int posDd = pos(array[d*3],newI,array);
    int iANDT = i & array[d*3-1];
    int final = iANDT * array[d*3-2];

    int pos = posDd + array[d*3-1] + final;

    return pos;

}

void buildVEBhelper(int* array, int height, int totalHeight, int start, bool isTop) {

    // Store: Size of bottom tree rooted at depth d B[d],
    // Size of the corresponding top tree T[d]
    // and depth of the corresponding top tree D[d]

    if(height == 1 && isTop && start != 1) {
        cout << "Aborted " << start << '\n';
        return;
    }

    int heightT = height / 2;
    int heightB = height / 2;
    if(height % 2 == 1) {
        heightB++;
    }
    int sizeB = pow(2,heightB)-1;
    int sizeT = pow(2,heightT)-1;

    int placement = (start+heightT)*3-2;

    cout << height << "," << heightT+start << "," << sizeB << "," << sizeT << "," << start << "," << placement << "," << isTop << '\n';

    array[(start+heightT)*3-2] = sizeB;
    array[(start+heightT)*3-1] = sizeT;
    array[(start+heightT)*3] = start;

    if(heightT >= 1) {
        cout << placement << " Top\n";
        buildVEBhelper(array,heightT,totalHeight,start, true);
    }
    if(heightB > 1) {
        cout << placement << " Bottom\n";
        buildVEBhelper(array,heightB,totalHeight,start+heightT, false);
    }


    /*if(height > 2) {
        buildVEBhelper(array,heightT,totalHeight,topHeight); // Top
        buildVEBhelper(array,heightB,totalHeight,topHeight+heightT); // Bottom
    }*/

}


void vebRecursiveSubBFS(int* bfs, int* ret, int x, int start, int h) {

    ret[start] = bfs[x];
    cout << "BFS placed " << bfs[x] << " at " << start << '\n';
    if(h != 1) {
        vebRecursiveSubBFS(bfs,ret,x*2,start*2,h-1); // Venstre barn
        vebRecursiveSubBFS(bfs,ret,x*2+1,start*2+1,h-1); // Højre barn
    }

}

void buildVEBBasedOnBFS(int* bfs, int* veb, int x, int y, int start) {

    if(x == y) {
        veb[start] = bfs[x];
        cout << "Placed " << bfs[x] << " at " << start << '\n';
        return;
    }
    int height = log2(y-x+1+1); // Antag at n = 2^x - 1
    int cut = height / 2;
    int n1 = pow(2,cut)-1; // Top tree
    int cut2 = height / 2;
    if(height % 2 == 1) {
        cut2++;
    }
    int n2 = pow(2,cut2)-1; // Bottom Trees

    cout << cut << "," << n1 << "," << cut2 << "," << n2 << "," << x << "," << y << '\n';

    int numberBottomTrees = (y-x+1-n1)/n2;

    // Top tree
    buildVEBBasedOnBFS(bfs,veb,x,x+n1-1,start);

    // Bottom trees
    // Byg midlertidige arrays og send ned
    for(int i = 0; i < numberBottomTrees; i++) {
        int* tempArray = new int[n2+1];
        int heightB = log2(n2+1);
        vebRecursiveSubBFS(bfs,tempArray,x+n1+i,1,heightB); // Virker for n=15
        //vebRecursiveSubBFS(bfs,tempArray,x*2+i,1,heightB); Virker for n=7
        //vebRecursiveSubBFS(bfs,tempArray,x+n1+n2*i,1,heightB);
        buildVEBBasedOnBFS(tempArray,veb,1,n2,start+n1+i*n2);
        delete(tempArray);
    }


}

void buildPointerVEBRecursive(int* bfs, int* veb, int* helper, int x, int y, int start, int depth, int bfspos, int n) {

    if(x == y) {
        veb[start*4-3] = bfs[x];
        if(depth == 1) {
            veb[start*4-2] = 0;
        }
        else {
            veb[start*4-2] = pos(depth-1,bfspos/2,helper)*4-3; // Parent
            cout << "Parent " << bfs[x] << "," << veb[start*4-2] << "," << bfspos << "," << depth-1 << '\n';
        }

        if(bfspos*2 > n) {
            veb[start*4-1] = 0;
        }
        else {
            veb[start*4-1] = pos(depth+1,bfspos*2,helper)*4-3; // Left
        }

        cout << "Trying to find " << bfspos*2 << " at depth="<< depth+1 << '\n';
        cout << "Found " << veb[start*4-1] << '\n';
        if (bfspos*2 + 1 > n) {
            veb[start*4] = 0; // Right
        }
        else {
            veb[start*4] = pos(depth+1,bfspos*2+1,helper)*4-3; // Right
        }
        cout << "Placed " << bfs[x] << " at " << start*4-3 << " - " << veb[start*4-2] << "," << veb[start*4-1] << "," << veb[start*4]  << "," <<  bfspos <<'\n';
        return;
    }
    int height = log2(y-x+1+1); // Antag at n = 2^x - 1
    int cut = height / 2;
    int n1 = pow(2,cut)-1; // Top tree
    int cut2 = height / 2;
    if(height % 2 == 1) {
        cut2++;
    }
    int n2 = pow(2,cut2)-1; // Bottom Trees

    cout << cut << "," << n1 << "," << cut2 << "," << n2 << '\n';

    int numberBottomTrees = (y-x+1-n1)/n2;

    // Top tree
    cout << depth << " Top\n";
    buildPointerVEBRecursive(bfs,veb,helper,x,x+n1-1,start,depth,bfspos,n);

    // Bottom trees
    // Byg midlertidige arrays og send ned
    cout << depth << "Bottom\n";
    int newBFSpos = bfspos << cut;
    for(int i = 0; i < numberBottomTrees; i++) {
        int* tempArray = new int[n2+1];
        int heightB = log2(n2+1);
        vebRecursiveSubBFS(bfs,tempArray,x+n1+i,1,heightB); // Virker for n=15
        //vebRecursiveSubBFS(bfs,tempArray,x*2+i,1,heightB); Virker for n=7
        //vebRecursiveSubBFS(bfs,tempArray,x+n1+n2*i,1,heightB);
        //buildPointerVEBRecursive(tempArray,veb,helper,1,n2,start+n1+i*n2,depth+cut,bfspos+n1+i*n2,n);
        //buildPointerVEBRecursive(tempArray,veb,helper,1,n2,start+n1+i*n2,depth+cut,pow(2,depth+cut-1)+i,n);
        //buildPointerVEBRecursive(tempArray,veb,helper,1,n2,start+n1+i*n2,depth+cut,pow(2,depth+cut-1)+i+((bfspos-pow(2,depth-1))*2),n);
        buildPointerVEBRecursive(tempArray,veb,helper,1,n2,start+n1+i*n2,depth+cut,newBFSpos+i,n);
        delete(tempArray);
    }

}

void buildDFSArray(int* dfsarray, int* array, int x, int y, int start) {

    int middle = (y-x)/2 + x;
    if((y-x) % 2 == 1) {
        middle++;
    }
    dfsarray[start] = array[middle];
    if(y > x) {
        buildDFSArray(dfsarray, array, x,middle-1,start+1); // Left
        if(y > x+1) {
            buildDFSArray(dfsarray,array,middle+1,y,start+(middle-x)+1); // Right
        }
    }

}

void buildDFSArrayPointer(int* dfsarray, int* array, int x, int y, int start, int parent) {

    int middle = (y-x)/2 + x;
    if((y-x) % 2 == 1) {
        middle++;
    }

    cout << middle << "," << x << "," << y << '\n';

    dfsarray[start] = array[middle];
    dfsarray[start+1] = parent;
    if(y > x) {
        dfsarray[start+2] = start+4;
        cout << "!!!\n";
    }
    else {
        dfsarray[start+2] = 0;
    }
    if(y > x+1) {
        dfsarray[start+3] = start+((middle-x)+1)*4;
    }
    else {
        dfsarray[start+3] = 0;
    }

    cout << middle << "," << parent << "," << start+4 << "," << start+((middle-x)+1)*4 << '\n';
    cout << dfsarray[start] << "," << dfsarray[start+1] << "," << dfsarray[start+2] << "," << dfsarray[start+3] << '\n';

    if(y>x) {
        buildDFSArrayPointer(dfsarray,array,x,middle-1,start+4,start);
        if(y>x+1) {
            buildDFSArrayPointer(dfsarray,array,middle+1,y,start+((middle-x)+1)*4,start);
        }
    }

}

void buildBFSPointerArray(int* parray, int* array, int n) {

    for(int i = 1; i <= n; i++) {
        parray[i*4-3] = array[i];
        parray[i*4-2] = i/2;
        parray[i*4-1] = i*2*4-3;
        parray[i*4] = (i*2+1)*4-3;
    }
}

void buildBFSArray(int* iarray, int* array, int x, int y, int start) {

    //cout << x << "," << y << "," << start << '\n';

    int middle = (y-x)/2 + x;
    if((y-x) % 2 == 1) {
        middle++;
        //cout << "middle++\n";
    }

    iarray[start] = array[middle];	// Value
    //cout << "Placed " << array[middle] << " at " << start << " with middle=" << middle << '\n';

    if(y > x) {
        buildBFSArray(iarray, array, x, middle-1, 2*start);
        if(y > x+1) buildBFSArray(iarray, array, middle+1, y, 2*start+1);
    }


}

void testObjectPointerImplicit(int* array, int r, int power) {

    cout << "Starting test\n";

    SYSTEMTIME  system_time;

    long tidStart, tidStop, totalTid;
    long* object = new long[power-10];
    long* pointer = new long[power-10];
    long* implicit = new long[power-10];
    int counter = 0;

    BinaryNode* fakeNode = NULL;


    for(int i = 10; i <= power; i++) {
        int x = pow(2,i);

        cout << "Building\n";
        BinaryNode* root = new BinaryNode(array,1,x,fakeNode);
        // BFS sorted arrays
        int* pointerArray = new int[x*4+1];
        int* implicitArray = new int[x+1];


        buildBFSArray(implicitArray,array,1,x,1);
        buildBFSPointerArray(pointerArray,implicitArray,x);

        cout << "Object\n";
        GetSystemTime(&system_time);
        tidStart = (long) (system_time.wHour*3600000) + (system_time.wMinute*60000) + (system_time.wSecond * 1000) + system_time.wMilliseconds;
        for(int j = 1; j <= r; j++) {

            // Find predecessor in object search tree
            bool b = true;
            BinaryNode* node = root;
            int goal = rand() % x +1;
            while(b) {
                if(node == NULL || node->value == goal) {
                    b = false;
                }
                else {
                    if(node->value < goal) {
                        node = node->right;
                    }
                    else {
                        node = node->left;
                    }
                }
            }

        }
        GetSystemTime(&system_time);
        tidStop = (long) (system_time.wHour*3600000) + (system_time.wMinute*60000) + (system_time.wSecond * 1000) + system_time.wMilliseconds;
        totalTid = tidStop - tidStart;
        object[counter] = totalTid;

        root->terminateMe();

        cout << "Pointer\n";
        GetSystemTime(&system_time);
        tidStart = (long) (system_time.wHour*3600000) + (system_time.wMinute*60000) + (system_time.wSecond * 1000) + system_time.wMilliseconds;
        for(int j = 1; j <= r; j++) {

            // Find predecessor in pointer array
            bool b = true;
            int start = 1;
            int goal = rand() % x +1;
            while(b) {
                if(pointerArray[start] == goal) {
                    b = false;
                }
                else {
                    if(pointerArray[start] < goal) {
                        start = pointerArray[start+3];
                    }
                    else {
                        start = pointerArray[start+2];
                    }
                }
            }

        }
        GetSystemTime(&system_time);
        tidStop = (long) (system_time.wHour*3600000) + (system_time.wMinute*60000) + (system_time.wSecond * 1000) + system_time.wMilliseconds;
        totalTid = tidStop - tidStart;
        pointer[counter] = totalTid;

        delete(pointerArray);

        cout << "Implicit\n";
        GetSystemTime(&system_time);
        tidStart = (long) (system_time.wHour*3600000) + (system_time.wMinute*60000) + (system_time.wSecond * 1000) + system_time.wMilliseconds;
        for(int j = 1; j <= r; j++) {

            // Find predecessor in implicit search tree
            bool b = true;
            int start = 1;
            int goal = rand() % x +1;
            while(b) {
                if(implicitArray[start] == goal) {
                    b = false;
                }
                else {
                    if(implicitArray[start] < goal) {
                        start = start*2+1;
                    }
                    else {
                        start = start*2;
                    }
                }
            }

        }
        GetSystemTime(&system_time);
        tidStop = (long) (system_time.wHour*3600000) + (system_time.wMinute*60000) + (system_time.wSecond * 1000) + system_time.wMilliseconds;
        totalTid = tidStop - tidStart;
        implicit[counter] = totalTid;

        delete(implicitArray);

        cout << "Run " << counter << " completed\n";
        counter++;

    }

    for(int i = 0; i < counter; i++) {
        //int p = pow(2,10+i);
        int p = 10+i;
        cout << p << '\t' << object[i] << '\t' << pointer[i] << '\t' << implicit[i] << '\n';
    }
}


/*void buildPointerVEB (int* toBuild, int* veb, int* helper, int n) {

    for(int i = 1; i <= n; i++) {
        int value = veb[i];
        int parent;
        if(i == 1) {
            parent = 0;
        }
        else {
            parent =
        }

        toBuild[i*4-3] = value;
        toBuild[i*4-2] = parent;
        toBuild[i*4-1] = left;
        toBuild[i*4] = right;
    }

}*/




int main(int argc, char* argv[]) {

    int n,r,power;
    if(argc != 3) {
        cout << "Syntax is <runs> <power>\n";
        //n = 4;
        r = 1000000;
        power = 24;
        n = pow(2,power);
    }
    else {
        //n = atoi(argv[1]);
        r = atoi(argv[1]);
        power = atoi(argv[2]);
        n = pow(2,power);
    }



    n = 15;

    // Sorted array
    int* array = new int[n+1];
    for(int i = 1; i <= n; i++) {
        array[i] = i;
    }

    /*
    BinaryNode* fakeNode = NULL;
    BinaryNode* root = new BinaryNode(array,1,n+1,fakeNode);

    // BFS sorted arrays
    int* pointerArray = new int[n*4+1];
    int* implicitArray = new int[n+1];

    //buildPointerArray(pointerArray, array, 1,n+1,1,-1);
    buildArray(implicitArray,array,1,n,1);
    buildPointerArray(pointerArray,implicitArray,n);
    */

    /*cout << "Printing " << n << '\n';

    for(int i = 1; i <= n; i++) {
        cout << i << " = " << implicitArray[i] << '\n';
    }

    cout << "Printing pointer\n";
    for(int i = 1; i <= n*4; i++) {
        if((i % 4) == 1) {
            cout << "---\n";
        }
        cout << i << " = " << pointerArray[i] << '\n';
    }*/

    /*BinaryNode* fakeNode = NULL;
    BinaryNode* root = new BinaryNode(array,1,5,fakeNode);
    cout << sizeof(root);*/

    int* bfs = new int[n+1];
    buildBFSArray(bfs,array,1,n,1);

    for(int i = 1; i <= n; i++) {
        cout << bfs[i] << '\n';
    }

    cout << "---\n";

    int* veb = new int[n+1];
    buildVEBBasedOnBFS(bfs,veb,1,n,1);

    for(int i = 1; i <= n; i++) {
        cout << veb[i] << '\n';
    }

    cout << "---\n";

    // Store: Size of bottom tree rooted at depth d B[d],
    // Size of the corresponding top tree T[d]
    // and depth of the corresponding top tree D[d]

    // buildVEBhelper(int* array, int height, int totalHeight, int topHeight) {
    int height = log2(n+1);
    int* helper = new int[height*3+1];
    buildVEBhelper(helper,height,height,1,false);

    for(int i = 1; i <= height*3; i++) {
        if((i % 3) == 1) {
            cout << "---\n";
        }
        cout << helper[i] << '\n';
    }

    cout << "---Test---\n";

    // i = BSF position, and d = depth
    // Pos[d] = Pos[D[d]] + T[d] + (i AND T[d]) * B[d]


    // Test 1: Fra i=3 til i=6 og i=7 (skal give res 3 og 6)
    int test = pos(3,7,helper);

    cout << test << '\n';

    test = pos(2,2,helper);

    cout << test << '\n';

    cout << "---VEB Pointer---\n";
    int* pointer = new int[n*4+1];
    buildPointerVEBRecursive(bfs,pointer,helper,1,n,1,1,1,n);

    for(int i = 1; i <= n*4; i++) {
        if((i % 4) == 1) {
            cout << "---\n";
        }
        cout << pointer[i] << '\n';
    }


    /*n = 8;
    int* dfsarray = new int[n+1];
    int* dfsPointerArray = new int[n*4+1];
    buildDFSArray(dfsarray,array,1,n,1);
    buildDFSArrayPointer(dfsPointerArray,array,1,n,1,0);

    for(int i = 1; i <= n; i++) {
        cout << dfsarray[i] << '\n';
    }

    cout << "-------------\n";

    for(int i = 1; i <= n*4; i++) {
        if(i % 4 == 1) {
            cout << "---\n";
        }
        cout << dfsPointerArray[i] << '\n';
    }*/

    //testObjectPointerImplicit(array,r,power);

    //int test = pow(2,20);
    /*int test = pow(2,25);

    BinaryNode* fakeNode = NULL;
    BinaryNode* root = new BinaryNode(array,1,test,fakeNode);
    root->printMe();
    root->terminateMe();*/

}
