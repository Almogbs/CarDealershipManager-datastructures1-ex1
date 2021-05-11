
#include "avl_tree.h"
#include <vector>
#include <ostream>
#include <random>
#include <chrono>
#include <algorithm>

int main(){
	AVLTree<int> tree;
	int a[5] = {3, 9, 21, 34, 55};
	AVLTree<int> tree2(a, 5);
	tree2.printInorder();
	//Specific Cases

	/*correct output for the four next cases
	1 BF: 0 Height: 0
	2 BF: 0 Height: 1
	3 BF: 0 Height: 0 */

	//basic LL root change
	tree.insert(3);
	tree.insert(2);
	tree.insert(1);
	tree.printInorder();
	tree.clear();

	//basic LR root change
	tree.insert(3);
	tree.insert(1);
	tree.insert(2);
	tree.printInorder();
	tree.clear();

	//basic RR root change
	tree.insert(1);
	tree.insert(2);
	tree.insert(3);
	tree.printInorder();
	tree.clear();

	//basic RL root change
	tree.insert(1);
	tree.insert(3);
	tree.insert(2);
	tree.printInorder();
	tree.clear();

	//basic LR not root change
	/*correct output
	1 BF: 0 Height: 0
	2 BF: 0 Height: 1
	3 BF: 0 Height: 0
	4 BF: 1 Height: 2
	5 BF: 0 Height: 0
	6 BF: 1 Height: 3
	7 BF: 0 Height: 0
	8 BF: 1 Height: 1 */
	tree.insert(6);
	tree.insert(4);
	tree.insert(8);
	tree.insert(3);
	tree.insert(5);
	tree.insert(7);
	tree.insert(1);
	tree.insert(2);
	tree.printInorder();
	tree.clear();

	//basic LL not root change
	/*correct output
	1 BF: 0 Height: 0
	2 BF: 0 Height: 1
	3 BF: 0 Height: 0
	4 BF: 1 Height: 2
	5 BF: 0 Height: 0
	6 BF: 1 Height: 3
	7 BF: 0 Height: 0
	8 BF: 1 Height: 1 */
	tree.insert(6);
	tree.insert(4);
	tree.insert(8);
	tree.insert(3);
	tree.insert(5);
	tree.insert(7);
	tree.insert(2);
	tree.insert(1);
	tree.printInorder();
	tree.clear();

	//basic RR not root change
	/*correct output
	 * 1 BF: 0 Height: 0
	2 BF: 0 Height: 1
	3 BF: 0 Height: 0
	5 BF: 1 Height: 2
	7 BF: 0 Height: 0*/
	tree.insert(5);
	tree.insert(7);
	tree.insert(1);
	tree.insert(2);
	tree.insert(3);
	tree.printInorder();
	tree.clear();

	//basic RL not root change
	/*correct output
	 * 1 BF: 0 Height: 0
	2 BF: 0 Height: 1
	3 BF: 0 Height: 0
	5 BF: 1 Height: 2
	7 BF: 0 Height: 0*/
	tree.insert(5);
	tree.insert(7);
	tree.insert(1);
	tree.insert(3);
	tree.insert(2);
	tree.printInorder();
	tree.clear();

	//root deletion no roll successor is a neighbour
	/*correct output
	2 BF: 0 Height: 0
	3 BF: 1 Height: 1
	5 BF: 1 Height: 2
	6 BF: 0 Height: 0*/
	tree.insert(4);
	tree.insert(5);
	tree.insert(3);
	tree.insert(2);
	tree.insert(6);
	tree.remove(4);
	tree.printInorder();
	tree.clear();

	//root deletion no roll successor is not a neighbour
	/*correct output
	2 BF: 0 Height: 0
	3 BF: 1 Height: 1
	5 BF: 0 Height: 2
	6 BF: 0 Height: 0
	7 BF: 0 Height: 1
	8 BF: 0 Height: 0 */
	tree.insert(4);
	tree.insert(3);
	tree.insert(7);
	tree.insert(2);
	tree.insert(5);
	tree.insert(8);
	tree.insert(6);
	tree.remove(4);
	tree.printInorder();
	tree.clear();

	//node deletion no roll successor is a neighbour case7
	/*correct output
	1 BF: 0 Height: 0
	2 BF: 1 Height: 1
	4 BF: 1 Height: 2
	5 BF: 0 Height: 0
	7 BF: 1 Height: 3
	8 BF: -1 Height: 1
	9 BF: 0 Height: 0*/
	tree.insert(7);
	tree.insert(3);
	tree.insert(8);
	tree.insert(2);
	tree.insert(4);
	tree.insert(9);
	tree.insert(5);
	tree.insert(1);
	tree.remove(3);
	tree.printInorder();
	tree.clear();

	//node deletion no roll successor is not a neighbour case8
	/*correct output
	1 BF: 0 Height: 0
	2 BF: 1 Height: 1
	5 BF: 0 Height: 2
	6 BF: 0 Height: 0
	7 BF: 0 Height: 1
	8 BF: 0 Height: 0
	9 BF: 0 Height: 3
	10 BF: 0 Height: 0
	12 BF: -1 Height: 2
	13 BF: -1 Height: 1
	14 BF: 0 Height: 0*/
	tree.insert(9);
	tree.insert(3);
	tree.insert(112);
	tree.insert(2);
	tree.insert(7);
	tree.insert(110);
	tree.insert(113);
	tree.insert(1);
	tree.insert(5);
	tree.insert(8);
	tree.insert(114);
	tree.insert(6);
	tree.remove(3);
	tree.printInorder();
	tree.clear();

	//node deletion causing LR case9
	/*correct output
	1 BF: 0 Height: 0
	2 BF: 0 Height: 1
	3 BF: 0 Height: 0
	4 BF: 0 Height: 2
	7 BF: -1 Height: 1
	8 BF: 0 Height: 0*/
	tree.insert(7);
	tree.insert(2);
	tree.insert(8);
	tree.insert(1);
	tree.insert(4);
	tree.insert(9);
	tree.insert(3);
	tree.remove(9);
	tree.printInorder();
	tree.clear();

	//node deletion causing LL case10
	/*correct output
	1 BF: 0 Height: 0
	2 BF: 1 Height: 1
	3 BF: 0 Height: 2
	4 BF: 0 Height: 0
	7 BF: 0 Height: 1
	8 BF: 0 Height: 0*/
	tree.insert(7);
	tree.insert(3);
	tree.insert(8);
	tree.insert(2);
	tree.insert(4);
	tree.insert(9);
	tree.insert(1);
	tree.remove(9);
	tree.printInorder();
	tree.clear();

	//node deletion causing RR case11
	/*correct output
	2 BF: 0 Height: 0
	3 BF: 0 Height: 1
	7 BF: 0 Height: 0
	8 BF: 0 Height: 2
	9 BF: -1 Height: 1
	10 BF: 0 Height: 0*/
	tree.insert(3);
	tree.insert(2);
	tree.insert(8);
	tree.insert(7);
	tree.insert(1);
	tree.insert(9);
	tree.insert(110);
	tree.remove(1);
	tree.printInorder();
	tree.clear();

	//node deletion causing RL case12
	/*correct output
	* 2 BF: 0 Height: 0
	3 BF: 1 Height: 1
	7 BF: 0 Height: 2
	10 BF: 0 Height: 0
	13 BF: 0 Height: 1
	14 BF: 0 Height: 0*/
	tree.insert(3);
	tree.insert(2);
	tree.insert(113);
	tree.insert(7);
	tree.insert(1);
	tree.insert(114);
	tree.insert(110);
	tree.remove(1);
	tree.printInorder();
	tree.clear();

	//double rotations RL and RR case 13
	/*correct output
	3 BF: 0 Height: 0
	5 BF: 0 Height: 1
	7 BF: 0 Height: 0
	9 BF: 0 Height: 2
	11 BF: 0 Height: 0
	13 BF: 1 Height: 1
	15 BF: 0 Height: 3
	17 BF: 0 Height: 0
	19 BF: -1 Height: 2
	21 BF: -1 Height: 1
	23 BF: 0 Height: 0*/
	tree.insert(9);
	tree.insert(3);
	tree.insert(115);
	tree.insert(1);
	tree.insert(7);
	tree.insert(113);
	tree.insert(119);
	tree.insert(5);
	tree.insert(111);
	tree.insert(117);
	tree.insert(221);
	tree.insert(223);
	tree.remove(1);
	tree.printInorder();
	tree.clear();

	//double rotations RR and RR case 14
	/*correct output
	3 BF: 0 Height: 0
	7 BF: 0 Height: 1
	8 BF: 0 Height: 0
	9 BF: 0 Height: 2
	11 BF: 0 Height: 0
	13 BF: 1 Height: 1
	15 BF: 0 Height: 3
	17 BF: 0 Height: 0
	19 BF: -1 Height: 2
	21 BF: -1 Height: 1
	23 BF: 0 Height: 0*/
	tree.insert(9);
	tree.insert(3);
	tree.insert(115);
	tree.insert(1);
	tree.insert(7);
	tree.insert(113);
	tree.insert(119);
	tree.insert(8);
	tree.insert(111);
	tree.insert(117);
	tree.insert(221);
	tree.insert(223);
	tree.remove(1);
	tree.printInorder();
	tree.clear();

	//double rotations RL and LL case 15
	/*correct output
	6 BF: 0 Height: 0
	7 BF: 1 Height: 1
	8 BF: 1 Height: 2
	9 BF: 0 Height: 0
	10 BF: 0 Height: 3
	11 BF: -1 Height: 1
	12 BF: 0 Height: 0
	15 BF: 0 Height: 2
	17 BF: 0 Height: 0
	18 BF: 0 Height: 1
	20 BF: 0 Height: 0
	 */
	tree.insert(115);
	tree.insert(110);
	tree.insert(220);
	tree.insert(8);
	tree.insert(111);
	tree.insert(117);
	tree.insert(221);
	tree.insert(7);
	tree.insert(9);
	tree.insert(112);
	tree.insert(118);
	tree.insert(6);
	tree.remove(221);
	tree.printInorder();
	tree.clear();

	//double rotations LR and LL case 16
	/*correct output
	6 BF: 0 Height: 0
	7 BF: 1 Height: 1
	8 BF: 1 Height: 2
	9 BF: 0 Height: 0
	10 BF: 0 Height: 3
	11 BF: -1 Height: 1
	12 BF: 0 Height: 0
	15 BF: 0 Height: 2
	20 BF: 0 Height: 0
	21 BF: 0 Height: 1
	22 BF: 0 Height: 0 */
	tree.insert(115);
	tree.insert(110);
	tree.insert(220);
	tree.insert(8);
	tree.insert(111);
	tree.insert(117);
	tree.insert(222);
	tree.insert(7);
	tree.insert(9);
	tree.insert(112);
	tree.insert(221);
	tree.insert(6);
	tree.remove(117);
	tree.printInorder();
	tree.clear();

	//delete node cause LR
	/*correct output
	 * 2 BF: 0 Height: 0
	3 BF: -1 Height: 2
	4 BF: 0 Height: 0
	6 BF: 1 Height: 1*/

	tree.insert(5);
	tree.insert(3);
	tree.insert(6);
	tree.insert(2);
	tree.insert(4);
	tree.remove(5);
	tree.printInorder();
	tree.clear();

	//delete node cause LR
	/*correct output
	 * 2 BF: 0 Height: 0
	3 BF: 0 Height: 1
	6 BF: 0 Height: 0*/
	tree.insert(5);
	tree.insert(3);
	tree.insert(6);
	tree.insert(2);
	tree.remove(5);
	tree.printInorder();
	tree.clear();

	return 0;
}
