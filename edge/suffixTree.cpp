#include "suffixTree.h"
/*
 * Using Contor pairing function which is basically a bijective mapping from N*N
 * -> N.
 */
 //
 // Cac chuc nang tim kiem chi lam trong 3 ham Printtopfive, patternAnalyze, task_manager
 // Xong tinh nang and or + -
 //
vector<string> ReadString; // luu lai nhung string da doc
vector<string> wordSearch; // cat cai tu can search thanh nhung doan nho, dung de search and or ...
vector<fileData> fileRank;
map<string, int>mp;
vector<string> vt[500];
vector<string> dict; // vector dung de load file lich su trong auto suggestion
int Node::noOfNodes = 1;
string Input; // need to fix to vector
int inputLength;
vector<vector<Edge>> v(20000);
unordered_map <int, vector<vector<Edge>>> multipleEdgeHash;
Node * nodeArray;
bool hSave; // dung de check xem co can luu tu vao history ko

void printTopFive(int searchType)
{
	vector<fileData> fD;
	int long2sung = 0;
	int pre = 100000;
	vector<int>top; // luu vi tri cua top 5
	for (int i = 0; i < fileRank.size() && i < 5; i++)
	{
		int max = 0;
		int pos = -1;
		for (int j = 0; j < fileRank.size(); j++)
		{
			if (fileRank[j].Totalscore == 0) continue;
			else if (fileRank[j].Totalscore > pre) continue;
			else if (fileRank[j].Totalscore == pre)
			{
				bool isok = true;
				for (int k = 0; k < top.size(); k++)
				{
					if (top[k] == j)
					{
						isok = false;
						break;
					}
				}
				if (isok)
				{
					max = fileRank[j].Totalscore;
					pos = j;
				}
			}
			else if (fileRank[j].Totalscore > max)
			{
				max = fileRank[j].Totalscore;
				pos = j;
			}
		}
		if (pos != -1)
		{
			fD.push_back(fileRank[pos]);
			top.push_back(pos);
		}
		pre = max;
	}
	for (int i = 0; i < 5 && i < fD.size(); i++)
	{
		color(249);
		cout << "\t\t\t" << fD[i].name << " : ";
		color(242);
		if (searchType == 0 || searchType == 3 || searchType == 9 || searchType == 4)
		{
			cout << fD[i].Totalscore << endl;
		}
		else if (searchType == 12)
		{
			color(252);
			cout << fD[i].score[0];
			color(240);
			cout << " -- ";
			color(243);
			cout << fD[i].score[1];
			color(240);
			cout << " -- ";
			color(253);
			cout << fD[i].score[2];
			color(240);
			cout << " -- ";
			color(246);
			cout << fD[i].score[3] << endl;
		}
		else if (searchType == 11) {
			if (fD[i].score[0] != 0) {
				cout << fD[i].Totalscore << endl;
			}
		}
		else
		{
			color(252);
			cout << fD[i].score[0];
			color(240);
			cout << " --- ";
			color(243);
			cout << fD[i].score[1] << endl;
		}
		if (searchType == 10) {//only for operation searchtype =10// updat 25/7
			if (fD[i].score[0] != 0 && fD[i].score[1] != 0) {
				cout << fD[i].score[0] + fD[i].score[1] << endl;
			}
		}
		for (int j = 0; j < wordSearch.size(); j++)
		{
			if (fD[i].score[j] != 0 || searchType == 4 && fD[i].Totalscore > 0)
			{
				int l = fD[i].position[j];
				while (ReadString[fD[i].no][l] != '.' && l != 0)
				{
					l--;
				}
				if (l != 0) l++;
				cout << "+ ";
				if (j == 0)
					color(252);
				else if (j == 1)
					color(243);
				else if (j == 2)
					color(253);
				else if (j == 3)
					color(246);
				cout << wordSearch[j] << ": ";
				color(240);
				while (ReadString[fD[i].no][l] == ' ') l++;
				int z = l;
				int first = l;
				vector<int> hight_light;
				/*if (fileRank[i].no == 5)
					cout << "";*/
				v = multipleEdgeHash[fD[i].no];
				findHightlight(wordSearch[j], fD[i].no, l, searchType, hight_light);
				sort(hight_light.begin(), hight_light.end());
				while (true) // bien chu dau tien trong cau thanh chu hoa
				{
					if (ReadString[fD[i].no][z] > 96 && ReadString[fD[i].no][z] < 123)
					{
						ReadString[fD[i].no][z] -= 32;
						long2sung = i;
						break;
					}
					z++;
				}
				int k = 0;
				int tmp;
				int colorWord = 0;
				int leng = wordSearch[j].length();
				int stop = 0;
				int range = hight_light.size();
				while (stop != 2 && l < ReadString[fD[i].no].size())
				{
					if (ReadString[fD[i].no][l] == '.')
						stop++;
					if (range > 0 && hight_light[k] - l == 0)
					{
						colorWord = leng;
						k++;
						range--;
					}
					if (colorWord > 0)
					{
						color(206);
						colorWord--;
					}
					else color(240);
					cout << ReadString[fD[i].no][l];
					l++;
				}
				cout << "." << endl;
				cout << endl;
				ReadString[fD[long2sung].no][z] += 32;
			}
		}
	}
	if (fD.size() == 0)
	{
		color(236);
		cout << "NOT FOUND" << endl;
		Sleep(2000);
	}
	else
	{
		cout << "Choose: ";
		int n;
		color(244);
		cin >> n;
		while (n > fD.size() || n <= 0) cin >> n;
		viewFileByNodepad(fD[n - 1].name);
	}
}
void viewFileByNodepad(string fpath)
{
	string topic = "notepad \"" + fpath + "\"";
	system(topic.c_str());
}
void patternAnalyze(string pattern, int &searchType) // dung cho cac chuc nang search
{
	if (pattern.size() == 0) return;
	wordSearch.clear();
	string search = "";
	// 12 tinh nang:
	// and : ok
	// or : ok
	// - : ok
	// intitle: ok
	// + : ok
	// filetype (bo)
	// $: ?
	// #: ?
	// " " : ok
	// * : A *, * A (ok) 
	// .. : ok
	// ~ : ok
	if (pattern[0] == '~')
	{
		pattern.erase(pattern.begin());
		searchType = 12;
		search = pattern;
		wordSearch.push_back(pattern);
	}
	if (pattern[0] == '#')
	{
		searchType = 9;
		search = pattern;
		wordSearch.push_back(pattern);
	}
	if (pattern[0] == '\"')
	{
		for (int j = 1; j < pattern.size(); j++)
		{
			if (pattern[j] == '\"')
			{
				searchType = 9;
				wordSearch.push_back(search); // luu danh sach nhung tu can search
				search = "";
				break;
			}
			else
				search += pattern[j];
		}
	}
	else if (pattern.size() > 2 && pattern[0] == '*' && pattern[1] == ' ') // truong hop * A -> giong voi exact search
	{
		searchType = 9;
		for (int i = 2; i < pattern.size(); i++)
		{
			search += pattern[i];
		}
		wordSearch.push_back(search);
	}
	else if (pattern.size() > 3 && pattern[pattern.size() - 2] == ' ' && pattern[pattern.size() - 1] == '*') // truong hop A * -> giong voi exact search
	{
		searchType = 9;
		for (int i = 0; i < pattern.size() - 2; i++)
		{
			search += pattern[i];
		}
		wordSearch.push_back(search);
	}
	else if (pattern.size() > 9 && pattern[0] == 'i') {
		if (pattern[1] == 'n'&&pattern[2] == 't'&&pattern[3] == 'i'&&pattern[4] == 't'&&pattern[5] == 'l'&&pattern[6] == 'e'&&pattern[7] == ':') {
			searchType = 4;//intitle search
			for (int i = 8; i < pattern.size(); i++) {
				search += pattern[i];
			}
			wordSearch.push_back(search);
		}
	}
	else
	{
		for (int i = 0; i < pattern.size(); i++)
		{

			if (pattern[i] == ' ')
			{
				if (i + 4 < pattern.size() - 1 && pattern[i + 1] == 'a' && pattern[i + 2] == 'n' && pattern[i + 3] == 'd' && pattern[i + 4] == ' ')
				{
					searchType = 1;
					wordSearch.push_back(search); // luu danh sach nhung tu can search
					search = "";
					i += 5;
					for (int j = i; j < pattern.size(); j++)
					{
						search += pattern[j];
					}
					wordSearch.push_back(search);
					break;
				}
				else if (i + 3 < pattern.size() - 1 && pattern[i + 1] == 'o' && pattern[i + 2] == 'r' && pattern[i + 3] == ' ')
				{
					searchType = 2;
					wordSearch.push_back(search); // luu danh sach nhung tu can search
					search = "";
					i += 4;
					for (int j = i; j < pattern.size(); j++)
					{
						search += pattern[j];
					}
					wordSearch.push_back(search);
					break;
				}
				else if (pattern.size() - i > 2 && pattern[i + 1] == '-')  //4
				{
					searchType = 3;
					wordSearch.push_back(search);
					search = "";
					i += 2;
					for (int j = i; j < pattern.size(); j++)
					{
						search += pattern[j];
					}
					wordSearch.push_back(search);
					break;
				}
				else if (pattern.size() - i > 2 && pattern[i + 1] == '+') // 5
				{
					searchType = 5;
					wordSearch.push_back(search);
					search = "";
					i += 2;
					for (int j = i; j < pattern.size(); j++)
					{
						search += pattern[j];
					}
					wordSearch.push_back(search);
					break;
				}
				else if (pattern.size() - i > 3 && pattern[i + 1] == '*' && pattern[i + 2] == ' ') {
					searchType = 10;
					wordSearch.push_back(search);
					search = "";
					i += 2;
					for (int j = i; j < pattern.size(); j++)
					{
						search += pattern[j];
					}
					wordSearch.push_back(search);
					break;
				}
				else if (pattern[i + 1] == '$')
				{
					int leng = pattern.size();
					int countDot = 0, count$ = 0;
					for (int j = i + 1; j < leng; j++)
					{
						if (pattern[j] == '.')
							countDot++;
						if (pattern[j] == '$')
							count$++;
					}
					string num;
					string num2;
					if (countDot == 0 && count$ == 1) {
						searchType = 7;
						wordSearch.push_back(search);
						search = "";
					}
					else if (countDot == 2 && count$ == 2)
					{
						int j;
						for (j = i + 2; pattern[j] != '.'; j++)
						{
							num += pattern[j];

						}
						j += countDot + 1;
						for (; pattern[j] != '\0'; j++)
						{
							num2 += pattern[j];
						}
						stringstream convertToNum(num);
						int x1;
						convertToNum >> x1;
						stringstream convertToNum2(num2);
						int x2;
						convertToNum2 >> x2;
						ofstream fout("Between.txt");
						fout << x1;
						fout << " ";
						fout << x2;
						fout.close();
						wordSearch.push_back(search);
						wordSearch.push_back("$");
						searchType = 11;
					}
				}
			}
			/*else if (pattern[i] = '.') {//chuc nang so 11 tim gia tri between
				if (pattern[i + 1] == '.') {
					searchType = 11;

					int val1 = stoi(search);//chuyen chuoi thanh so
					search="";
					for (int j = i + 2; j < pattern.size(); j++) {
						search += pattern[j];
					}
					int val2 = stoi(search);
					for (int j = val1; j <= val2; j++) {
						string s = to_string(j);// chuyen so thanh chuoi trong khoang can tim
						wordSearch.push_back(s);
					}
					break;
				}
			}*/
			else
			{
				search += pattern[i];
			}
		}
	}
	if (searchType == 0 || searchType == 7) wordSearch.push_back(search);
	if (searchType == 12) findSynonyms(pattern, searchType);
}

long Edge::returnHashKey(int nodeID, int c) {
	return (long)(nodeID + (((long)c << 90000))); // dịch chuyển nhị phân 90000
}

void Edge::reBulid(int node, int c) {
	this->key = Edge::returnHashKey(node, c);
	v[startNode].push_back(*this);
}

void Edge::insert() {
	v[startNode].push_back(*this);
}

void Edge::remove() {
	int size = v[startNode].size();
	for (int i = 0; i < size; i++)
	{
		if (v[startNode][i].key == key)
		{
			v[startNode].erase(v[startNode].begin() + i);
			return;
		}
	}	//xoa nhanh cu
}

Edge Edge::findEdge(int node, int c) {
	long key = returnHashKey(node, c);
	int size = v[node].size();
	for (int i = 0; i < size; i++)
	{
		if (v[node][i].key == key)
			return v[node][i];
	}
	return Edge();
}

// rootNode should be equal to the closest node to the end of the tree so this can be used in the next iteration.
void suffixTree::migrateToClosestParent() {// di chuyen rootNode toi parent gan nhat
	// If the current suffix tree is not ending on a node, this condition is already
	if (!endReal()) {
		Edge e = Edge::findEdge(rootNode, Input[startIndex]); // find the next extension path of Edge
		//assert(e.startNode != -1);
		int labelLength = e.endLabelIndex - e.startLabelIndex; // number of character in the Edge
		// Go down
		while (labelLength <= (endIndex - startIndex)) {
			startIndex += labelLength + 1;
			rootNode = e.endNode;
			if (startIndex <= endIndex) {
				e = Edge::findEdge(e.endNode, Input[startIndex]);
				//testing code
				/*if (e.startNode == -1) {
					for (int a = 0; a < 10; a++)
						cout << Input[startIndex + a] << endl;
					cout << startIndex << endl;
				}*/
				//	assert(e.startNode != -1);
				labelLength = e.endLabelIndex - e.startLabelIndex;
			}
		}
	}
}

/*
 * Break an edge so as to add new string at a specific point.
 */
int breakEdge(suffixTree &s, Edge &e, int lastIndex) {//build tree
	// Remove the edge 
	e.remove();
	Edge newEdge(s.rootNode, e.startLabelIndex,
		e.startLabelIndex + s.endIndex - s.startIndex, Edge::returnHashKey(s.rootNode, Input[e.startLabelIndex])); // create new breaking Edge from parentNode
	newEdge.insert();
	// Add the suffix link for the new node.
	nodeArray[newEdge.endNode].suffixNode = s.rootNode;
	e.startLabelIndex += s.endIndex - s.startIndex + 1; // modify parentNode agian
	e.startNode = newEdge.endNode;
	e.reBulid(e.startNode, Input[e.startLabelIndex]);
	return newEdge.endNode;
}

void buildSufffixTree(suffixTree &tree, int lastIndex) {
	int parentNode;
	// to keep track of the last encountered node.
	// Used for creating the suffix link.
	int previousParentNode = -1;
	while (true) {
		Edge e;
		parentNode = tree.rootNode;
		if (tree.endReal()) {
			e = Edge::findEdge(tree.rootNode, Input[lastIndex]);  // Check the substring exist or not
			if (e.startNode != -1)
				break;
		}
		// If previoustree ends in between an edge -> find that edge and match after that. 
		else {
			e = Edge::findEdge(tree.rootNode, Input[tree.startIndex]);
			int diff = tree.endIndex - tree.startIndex;
			if (Input[e.startLabelIndex + diff + 1] == Input[lastIndex])
				// We have a match
				break;
			//If match was not found this way, then we need to break this edge and add a node and insert the string.
			parentNode = breakEdge(tree, e, lastIndex);
		}

		// We have not matchng edge at this point, so we need to create a new
		// one, add it to the tree at parentNode position and then insert it
		// into the vector with hashkey
		// We are creating a new node here, which means we also need to update
		// the suffix link here. Suffix link from the last visited node to the
		// newly created node.
		Edge newEdge(parentNode, lastIndex, inputLength, Edge::returnHashKey(parentNode, Input[lastIndex])); // create one Edge
		newEdge.insert(); // insert to vector
		if (previousParentNode > 0) // update previous parent node
			nodeArray[previousParentNode].suffixNode = parentNode;
		previousParentNode = parentNode;
		// Move to next suffix, noOfMap.e. next extension.
		if (tree.rootNode == 0)
			tree.startIndex++;
		else {
			tree.rootNode = nodeArray[tree.rootNode].suffixNode; // using suffix link for fast inserting
		}
		tree.migrateToClosestParent(); // go to closest edge to modify it
	}

	if (previousParentNode > 0) // update previous parent node
		nodeArray[previousParentNode].suffixNode = parentNode;
	tree.endIndex++;
	tree.migrateToClosestParent();
}

int edgeLength(Edge e) // chieu dau cua nhanh
{
	return e.endLabelIndex - e.startLabelIndex + 1;
}

int countLeafByDFS(Edge e, int startNode, string t, int &wordApp, int &firstFound, int searchType, int patternLength, string pattern)
{
	if (t[e.endLabelIndex] == 'A') // nếu Edge là leaf
	{
		if (searchType == 9 || searchType == 7) {
			if (e.leafLabelIndex != 0 && t[e.leafLabelIndex - 1] > 96 && t[e.leafLabelIndex - 1] < 123)
				return 0;
			if (t[e.leafLabelIndex + patternLength] > 96 && t[e.leafLabelIndex + patternLength] < 123)
				return 0;
			if (e.leafLabelIndex != 0 && t[e.leafLabelIndex - 1] > 47 && t[e.leafLabelIndex - 1] < 58)
				return 0;
			if (t[e.leafLabelIndex + patternLength] > 47 && t[e.leafLabelIndex + patternLength] < 58)
				return 0;
			if (searchType == 7) {
				string tmp = "";
				for (int i = e.leafLabelIndex; i < e.leafLabelIndex + patternLength; i++)
					tmp += t[i];
				if (pattern.compare(tmp) != 0)
					return 0;
			}
		}
		if (t[e.leafLabelIndex] == '$' && searchType == 11)
		{
			string number;
			for (int i = e.leafLabelIndex + 1; t[i] > 47 && t[i] < 58; i++)
				number += t[i];
			stringstream convertToNum(number);
			int x;
			convertToNum >> x;
			ifstream fin("Between.txt");
			int range1;
			int range2;
			fin >> range1;
			fin >> range2;
			if (x < range1 || x >range2)
				return 0;
		}
		//cout << "Start at: " << e.leafLabelIndex << endl;
		wordApp++;
		if (firstFound > e.leafLabelIndex) firstFound = e.leafLabelIndex;
		return 1;
	}
	int count = 0;
	int size = v[startNode].size();
	for (int i = 0; i < size; i++) // go through all children which have the same startNode
	{
		count += countLeafByDFS(v[startNode][i], v[startNode][i].endNode, t, wordApp, firstFound, searchType, patternLength, pattern); // adding by if we find down
	}
	return count;
}

void setSuffixIndexByDFS(Edge &e, int labelHeight, int startNode)
{
	if (Input[e.endLabelIndex] == 'A') // nếu Edge là leaf
	{
		e.leafLabelIndex = inputLength + 1 - labelHeight - edgeLength(e); // đặt nhãn cho Edge
		return;
	}
	//for (int i = 0; i < arrayOfChildren[noOfMap].child[startNode].size(); i++)
	int size = v[startNode].size();
	for (int i = 0; i < size; i++)
	{
		Edge &a = v[startNode][i]; // bắt đầu dao sau
		setSuffixIndexByDFS(a, labelHeight + edgeLength(e), a.endNode);
	}
}

void setSuffixIndex() { // Go from root 0 to find
	int labelHeight = 0; // index của lá
	int size = v[0].size();
	for (int i = 0; i < size; i++) //go through all children which have the startNode = 0
	{
		Edge &e = v[0][i];
		setSuffixIndexByDFS(e, labelHeight, e.endNode); // go util meet the leaf
	}
}

int isMatched(string pattern, Edge e, int len, int &index, int &matchIndex, int noOfMap, string t) // Check if pattern is exist
{
	int i = e.startLabelIndex;
	int j = index;
	for (; i <= e.endLabelIndex && j < len; i++, j++)
	{
		if (pattern[j] != t[i])
			return -1; // 2 phần tử khác khác nhau -> không tìm thấy
		matchIndex++;
	}
	if (matchIndex == len)
		return 1; // đã tìm thấy hết pattern
	index = j;
	return 0; // pattern still not be compared fully -> return 0 -> go deeply
}

int frequency(string pattern, int noOfMap, int &wordApp, int &firstFound, int searchType) // tính số lần xuất hiện của pattern
{
	int len = pattern.length(); // chiều dài của từ cần tìm
	Edge e = Edge::findEdge(0, pattern[0]); // tìm từ đầu tiên của pattern từ ROOT (StartNode = 0)
	if (e.startNode != -1) // Nếu Edge có tồn tại
	{
		int matchIndex = 0; // số từ chính xác bắt đầu là 0
		int index = 0;
		int res = 1;
		string t = ReadString[noOfMap];
		res = isMatched(pattern, e, len, index, matchIndex, noOfMap, t); // kiểm tra độ chính xác của 2 từ
		do
		{
			if (res == 1) //tìm thấy
			{
				return countLeafByDFS(e, e.endNode, t, wordApp, firstFound, searchType, len, pattern); // trường hợp từ xuất hiện nhiều hơn 1 lần 
			}
			while (!res)// tiếp tục tìm từ từ giống nhau gần nhất, nếu tìm được hoặc không tìm được sẽ dừng
			{
				e = Edge::findEdge(e.endNode, pattern[index]); // tìm từ cạnh tiếp theo
				if (e.startNode == -1)return 0;
				res = isMatched(pattern, e, len, index, matchIndex, noOfMap, t); // tiếp tục so sánh 2 chuỗi
			}
		} while (res == 1); // khi tìm được 

	}
	return 0; // không tìm thấy
}

int patternPriority(int searchType)
{
	int a = searchType;
	if (a == 1 || a == 9 || a == 7 || a == 11 || a == 4 || a == 5)
		return 5;
	return 20;
}

void printAllEdges(int option) { // print all Edge contain in vector
	int count = 0;
	if (option)
	{
		cout << "StartNode\tEndNode\tSuffixLink\tFirstIndex\tlastIndex\tString" << endl;
		int size2 = v.size();
		for (int i = 0; i < size2; i++)
		{
			int size = v[i].size();
			for (int j = 0; j < size; j++)
			{
				Edge tmp = v[i][j];
				cout << tmp.startNode << "\t\t" << tmp.endNode
					<< "\t\t" << nodeArray[tmp.endNode].suffixNode
					<< "\t\t" << tmp.startLabelIndex
					<< "\t\t" << tmp.endLabelIndex
					<< "\t\t";
				count++;
				int head;
				if (inputLength > tmp.endLabelIndex)
					head = tmp.endLabelIndex;
				else
					head = inputLength;
				for (int i = tmp.startLabelIndex; i < head + 1; i++)
					cout << Input[i];
				cout << endl;
			}
		}
		cout << "Total edges: " << count << endl;
		return;
	}
	int size2 = v.size();
	for (int i = 0; i < size2; i++)
	{
		int size = v[i].size();
		for (int j = 0; j < size; j++)
		{
			count++;
		}
	}
	cout << "Total edges: " << count << endl;
}

void convertToLower(string &s, int &len) // convert string to lower and delete error character
{
	for (int i = 0; i < len; i++)
	{
		if (s[i] < 32 || s[i] > 126)
		{
			s.erase(i, 1);
			len--;
		}
		else
			s[i] = tolower(s[i]);
	}
}

void reUseVector(vector<vector<Edge>>&vec, int len, int sizeOfVec) //clear vector and resize for next use
{
	int length = vec.size();
	for (int i = 0; i < length; i++)
		vec[i].clear();
	vec.resize(len * 2 + 1);
	//vec.shrink_to_fit();
	//	vector<vector<Edge>> tmp(10000);
	//	v.swap(tmp);
}

void task_manager(int &noOfMap) // duyệt nhiều file
{
	int sizeOfVec = 0;
	string path = "file";
	auto start = std::chrono::high_resolution_clock::now(); // bắt đầu đếm giờ
	int phase = 0;
	for (const auto & entry : fs::directory_iterator(path))
	{
		// Start timer.
		cout << "Handling file: " << entry << " at " << phase << endl;
		phase++;
		Node::noOfNodes = 1;// reset noOfNodes
		Input = ""; // lấy thông tin từ file
		string word;
		ifstream fin;
		fin.open(entry.path());
		fs::path path = entry.path(); // chuyen doi cai file name ve dang string de xuat ra sau khi search
		string filepath = path.u8string();
		while (!fin.eof())
		{
			fin >> word;
			Input += word;
			Input += " ";
		}
		int c = 0, l = Input.length() - 1;
		fin.close();
		inputLength = Input.length();
		convertToLower(Input, inputLength); // đổi tất cả thành chữ thường
		Input.back() = 'A';
		inputLength--;
		reUseVector(v, inputLength, sizeOfVec);
		nodeArray = (Node *)malloc(2 * inputLength*(sizeof(Node)));
		suffixTree tree(0, 0, -1);
		for (int i = 0; i <= inputLength; i++)
			buildSufffixTree(tree, i);
		sizeOfVec = Node::noOfNodes;
		// bắt đầu đánh dấu index của lá
		setSuffixIndex();
		delete nodeArray;
		multipleEdgeHash[noOfMap] = v;

		ReadString.push_back(Input);
		fileRank.push_back({ noOfMap,0,filepath,0,0,0 }); // luu lai file path, khoi tao diem cho no la 1
		noOfMap++;
	}
	auto end = std::chrono::high_resolution_clock::now(); // kết thúc đếm giờ
	long long int time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
	cout << "Total time taken to build: " << time * 1.0 / 1000000000
		<< "<s>" << endl;
	//system("pause");
	//	introduce();
	clear();
	loadHistory();
	BuildSynonymsList();
	color(240);
	VeKhungCH(11, 9, 44, 3, 1);
	gotoxy(20, 10);
	cout << "Enter patterns to search..." << endl;
	color(240);
	gotoxy(20, 11);
	cout << "Search for: ";
	color(243);
	string pattern;
	inputData(pattern);
	while (pattern == "") inputData(pattern);
	int searchType = 0;
	int len2 = pattern.length();
	convertToLower(pattern, len2);
	DeleteStopWord(pattern);
	color(240);
	cout << endl;
	cout << "\t         After deleted stop words: ";
	color(242);
	cout << pattern << endl;
	patternAnalyze(pattern, searchType);
	int f;
	while (pattern.compare("exit")) { // demo search
	//	auto start2 = std::chrono::high_resolution_clock::now(); // bắt đầu đếm giờ
		double begin = clock();
		int top = patternPriority(searchType);
		int top20 = 0;
		for (int num = 0; num < noOfMap; num++)
		{
			f = 0;
			int wA[4];//mang chua vi tri xuat hien
			v = multipleEdgeHash.at(num);
			for (int i = 0; i < wordSearch.size(); i++) {
				int wordApp = 0;
				int firstFound = 100000;
				f = frequency(wordSearch[i], num, wordApp, firstFound, searchType);
				wA[i] = wordApp;
				if (firstFound == 100000)fileRank[num].position[i] = 0;
				else fileRank[num].position[i] = firstFound; // t them mang position vo trong struct luon r, @@ vay cho no dong bo, sr,sr
			}
			if (searchType == 0 || searchType == 9) // search thong thuong
			{
				fileRank[num].Totalscore = wA[0]; // cap nhat diem so cho file
				fileRank[num].score[0] = wA[0];
			}
			else if (searchType == 1 || searchType == 7) // and operation , + operation
			{
				fileRank[num].score[0] = wA[0];
				fileRank[num].score[1] = wA[1];
				if (wA[0] != 0 && wA[1] != 0)
					fileRank[num].Totalscore = wA[0] + wA[1];
				else fileRank[num].Totalscore = 0;

			}
			else if (searchType == 2) // or operation
			{
				fileRank[num].score[0] = wA[0];
				fileRank[num].score[1] = wA[1];
				fileRank[num].Totalscore = wA[0] + wA[1];
			}
			else if (searchType == 3) // - operation
			{
				if (wA[1] != 0)
				{
					fileRank[num].Totalscore = 0;
				}
				else
				{
					fileRank[num].Totalscore = wA[0];
					fileRank[num].score[0] = wA[0];
				}

			}
			else if (searchType == 4) {//search intitle operation
				if (fileRank[num].position[0] < 100)//xuat hien trong title
					fileRank[num].Totalscore = wA[0];
				else fileRank[num].Totalscore = 0;
			}
			else if (searchType == 5) // + operation
			{
				fileRank[num].score[0] = wA[0];
				fileRank[num].score[1] = wA[1];
				if (wA[0] != 0 && wA[1] != 0)
					fileRank[num].Totalscore = wA[0] + wA[1];
				else fileRank[num].Totalscore = 0;
			}
			else if (searchType == 10) {//* operation
				fileRank[num].score[0] = wA[0];
				fileRank[num].score[1] = wA[1];

				if (wA[0] != 0 && wA[1] != 0)
					fileRank[num].Totalscore = wA[0] + wA[1];
				else fileRank[num].Totalscore = 0;
			}
			else if (searchType == 11) {//between number operation
				fileRank[num].score[0] = wA[0];
				fileRank[num].score[1] = wA[1];
				if (wA[0] != 0 && wA[1] != 0)
					fileRank[num].Totalscore = wA[0] + wA[1];
				else fileRank[num].Totalscore = 0;

			}
			else if (searchType == 12) // synonyms
			{
				fileRank[num].score[0] = wA[0];
				fileRank[num].score[1] = wA[1];
				fileRank[num].score[2] = wA[2];
				fileRank[num].score[3] = wA[3];
				fileRank[num].Totalscore = wA[0] + wA[1] + wA[2] + wA[3];
			}
			if (fileRank[num].Totalscore > 0)
			{
				top20++;
				if (top20 >= top)
					break;
			}

		}

		//auto end2 = std::chrono::high_resolution_clock::now(); // kết thúc đếm giờ
		//int time2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end2 - start2).count();
		double end = clock();
		double elapsed_secs = (end - begin) / phase;

		//cout << char(34) << tmp << char(34) << " turn on : " << f << " time." << endl;
	//	gotoxy(18, 17);

		color(252);
		VeKhungCH(18, 19, 30, 2, 1);
		gotoxy(24, 20);
		cout << "*-Top Five Result-*" << endl;
		cout << endl;
		color(242);
		cout << "\t\t    ";
		color(206);
		cout << "Time to search is:  " << elapsed_secs << "(s)" << endl;
		color(250);
		printTopFive(searchType);

		searchType = 0;
		//	cout << "----------------------------------------------------" << endl;
		clear();
		saveHistory(pattern);
		pattern = "";
		cin.clear();
		clear();
		color(240);
		VeKhungCH(11, 9, 44, 3, 1);
		gotoxy(20, 10);
		cout << "Enter patterns to search..." << endl;
		color(240);
		gotoxy(20, 11);
		cout << "Search for: ";
		color(243);
		inputData(pattern);
		while (pattern == "") inputData(pattern);
		len2 = pattern.length();
		convertToLower(pattern, len2);
		cout << endl;
		DeleteStopWord(pattern);
		color(240);
		cout << "\t         After deleted stop words: ";
		color(242);
		cout << pattern << endl;
		patternAnalyze(pattern, searchType);
		for (int i = 0; i < fileRank.size(); i++)
		{
			fileRank[i].Totalscore = 0;
			for (int j = 0; j < 4; j++)
			{
				fileRank[i].score[j] = 0;
				fileRank[i].position[j] = 0;
			}
		}
	}
}

void DeleteStopWord(string &pattern) {
	string SW, tmp;
	for (int i = 0; i < pattern.length(); i++) {
		if (pattern[i] == ' ') {
			for (int j = 0; j <= i; j++)
				tmp += pattern[j];
			ifstream fin("StopWord.txt");
			while (SW != tmp)
			{
				if (fin.eof()) return;
				SW.clear();
				getline(fin, SW);
				SW += " ";
			}
			if (SW == tmp)
			{
				pattern.erase(0, i + 1);
				i = 0;
				tmp.clear();
				pattern.erase(0, i);
				tmp.clear();
				fin.seekg(ios::beg);
			}
			fin.close();
		}
	}
}
void BuildSynonymsList()
{
	ifstream fin;
	fin.open("synonym.txt");
	string a, b, c, d;
	int i = 0;
	if (!fin.is_open()) return;
	while (!fin.eof())
	{
		getline(fin, a, ' ');
		getline(fin, b, ' ');
		getline(fin, c, ' ');
		getline(fin, d);
		mp.insert({ a,i });
		vt[i].push_back(b);
		vt[i].push_back(c);
		vt[i].push_back(d);
		i++;
	}
	fin.close();
}
void findSynonyms(string pattern, int &searchType)
{
	color(240);
	if (mp.find(pattern) == mp.end())
	{
		cout << "\t\tThe word " << pattern << " doesn't exist in our list." << endl;
		searchType = 0;
	}
	else
	{
		int t = mp[pattern];
		cout << "\t\tThe synonyms of " << pattern << " : ";
		color(236);
		for (int i = 0; i < vt[t].size(); i++)
		{
			wordSearch.push_back(vt[t][i]);
			cout << vt[t][i] << " ";
		}
		cout << endl;
	}
}
//void gotoxy(int x, int y)
//{
//	static HANDLE h = NULL;
//	if (!h)
//		h = GetStdHandle(STD_OUTPUT_HANDLE);
//	COORD c = { x, y };
//	SetConsoleCursorPosition(h, c);
//}
void inputData(string &p)
{
	int i = 0;
	char a;//a Temp char
	while (1)//infinite loop
	{
		gotoxy(35 + i, 11);
		a = _getch();//stores char typed in a
		if ((a >= 'a'&&a <= 'z') || (a >= 'A' && a <= 'Z') || (a >= '0'&&a <= '9') || a == '#' || a == '*' || a == '+' || a == '-' || a == '~' || a == '$' || a == '.' || a == '"' || a == ':' || a == 32)
			//check if a is numeric or alphabet
		{
			p.push_back(a);//stores a in pass
			++i;
			cout << a;
		}
		if (a == '\b'&&i >= 1)//if user typed backspace
			//i should be greater than 1.
		{
			cout << "\b \b";//rub the character behind the cursor.
			p.pop_back();
			--i;
		}
		if (a == 13)//if enter is pressed
		{
			cout << endl;
			break;//break the loop
		}
		if (a == 27) ExitProcess(0);
		autoSuggestion(p);
	}
}
void autoSuggestion(string p)
{
	int cnt = 0;
	hSave = false; // dung de check xem co can phai luu tu vao history ko
	for (int i = 0; i < 5; i++)
	{
		gotoxy(35, 14 + i);
		cout << "                                            ";
	}
	color(252);
	for (vector<string>::iterator it = dict.begin(); it != dict.end() && cnt < 5; ++it)
	{
		if (p == (*it).substr(0, p.length()))
		{
			gotoxy(35, 14 + cnt);
			cout << (*it) << endl;
			cnt++;
		}
	}
	if (cnt != 0) hSave = true;
	color(242);
}
bool loadHistory()
{
	ifstream fin;
	string word;
	fin.open("history.txt");
	if (!fin.is_open()) return false;
	while (!fin.eof())
	{
		getline(fin, word);
		if (word != "" && word != "\n")
			dict.push_back(word);
	}
	fin.close();
	return true;
}
void saveHistory(string pattern)
{
	if (!hSave)
	{
		ofstream fout;
		fout.open("history.txt", ios::app);
		fout << pattern << endl;
		fout.close();
	}
}
void hightLight(Edge e, int startNode, string t, int &firstFound, int searchType, int patternLength, vector<int> &a)
{
	if (t[e.endLabelIndex] == 'A') // n?u Edge là leaf
	{
		if (searchType == 9) {
			if (e.leafLabelIndex != 0 && t[e.leafLabelIndex - 1] > 96 && t[e.leafLabelIndex - 1] < 123)
				return;
			if (t[e.leafLabelIndex + patternLength] > 96 && t[e.leafLabelIndex + patternLength] < 123)
				return;
		}
		//cout << "Start at: " << e.leafLabelIndex << endl;
		if (firstFound <= e.leafLabelIndex)
			a.push_back(e.leafLabelIndex);
		return;
	}
	int count = 0;
	int size = v[startNode].size();
	for (int i = 0; i < size; i++) // go through all children which have the same startNode
	{
		hightLight(v[startNode][i], v[startNode][i].endNode, t, firstFound, searchType, patternLength, a); // adding by if we find down
	}
}
void findHightlight(string pattern, int noOfMap, int &firstFound, int searchType, vector<int> &a) // tính s? l?n xu?t hi?n c?a pattern
{
	int len = pattern.length(); // chi?u dài c?a t? c?n t?m
	Edge e = Edge::findEdge(0, pattern[0]); // t?m t? đ?u tiên c?a pattern t? ROOT (StartNode = 0)
	if (e.startNode != -1) // N?u Edge có t?n t?i
	{
		int matchIndex = 0; // s? t? chính xác b?t đ?u là 0
		int index = 0;
		int res = 1;
		string t = ReadString[noOfMap];
		res = isMatched(pattern, e, len, index, matchIndex, noOfMap, t); // ki?m tra đ? chính xác c?a 2 t?
		do
		{
			if (res == 1) //t?m th?y
			{
				hightLight(e, e.endNode, t, firstFound, len, searchType, a); // trư?ng h?p t? xu?t hi?n nhi?u hơn 1 l?n 
				return;
			}
			while (!res)// ti?p t?c t?m t? t? gi?ng nhau g?n nh?t, n?u t?m đư?c ho?c không t?m đư?c s? d?ng
			{
				e = Edge::findEdge(e.endNode, pattern[index]); // t?m t? c?nh ti?p theo
				if (e.startNode == -1)return;
				res = isMatched(pattern, e, len, index, matchIndex, noOfMap, t); // ti?p t?c so sánh 2 chu?i
			}
		} while (res == 1); // khi t?m đư?c 

	}
}