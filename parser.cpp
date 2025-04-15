#include <bits/stdc++.h>
using namespace std;

// Precedence matrix
int precedence[8][8] = {
    {-1, -1, 1, 1, -1, 1, -1, 1},     // +
    {-1, -1, 1, 1, -1, 1, -1, 1},     // -
    {-1, -1, -1, -1, -1, 1, -1, 1},   // *
    {-1, -1, -1, -1, -1, 1, -1, 1},   // /
    {1, 1, 1, 1, 0, 1, 1, 1},         // $
    {1, 1, 1, 1, -1, 1, 1, 1},        // (
    {-1, -1, -1, -1, -1, -1, -1, -2}, // )
    {-1, -1, -1, -1, -1, -1, -1, -2}  // number
};

// 1 shift, -1 reduce, 0 accept, -2 invalid string

// Mapping symbols to indices
map<char, int> symbolIndex = {
    {'+', 0}, {'-', 1}, {'*', 2}, {'/', 3}, {'$', 4}, {'(', 5}, {')', 6}};
// Mapping indices to symbols
map<int, char> indexToSymbol = {
    {0, '+'}, {1, '-'}, {2, '*'}, {3, '/'}, {4, '$'}, {5, '('}, {6, ')'}, {7, 'n'}};

// Function to get symbol index
int getIndex(char c)
{
    if (c == 'n')
        return 7; // Number (int literal)
    if (c == 'E')
        return -1; // Non-terminal E (not in precedence matrix)
    return symbolIndex[c];
}

// Function to get second-top stack element for precedence check
char getSecondTop(stack<char> &stk)
{
    if (stk.size() < 2)
        return stk.top(); // Fallback to top if no second-top
    char top = stk.top();
    stk.pop();
    char second = stk.top();
    stk.push(top);
    return second;
}

// Function to print stack contents
void printStack(stack<char> stk)
{
    vector<char> temp;
    while (!stk.empty())
    {
        temp.push_back(stk.top());
        stk.pop();
    }
    cout << "Stack: ";
    if (temp.empty())
        cout << "(empty)";
    else
    {
        reverse(temp.begin(), temp.end());
        for (char c : temp)
            cout << c << " ";
    }
}

// Function to print input buffer from current pointer
void printInputBuffer(const string &input, int i)
{
    cout << "Input Buffer: ";
    if (i < input.length())
        cout << input.substr(i);
    else
        cout << "(empty)";
    cout << endl;
}

// Function to perform reductions based on grammar
bool tryReduce(stack<char> &stk)
{
    vector<char> temp;
    while (!stk.empty())
    {
        temp.push_back(stk.top());
        stk.pop();
    }
    reverse(temp.begin(), temp.end());

    // Reduction on the based of Grammer:---------------------------------------------------------------------------
    // {E -> E + E, E -> E - E, E -> E * E, E -> E / E, E -> int literal, E -> (E)}

    // For E -> int literal
    if (temp.size() >= 1 && temp.back() == 'n')
    {
        cout << "Reduce: E -> int literal" << endl;
        for (int i = 0; i < temp.size() - 1; i++)
            stk.push(temp[i]);
        stk.push('E'); // Push E for reduced expression
        return true;
    }
    // For E -> E + E, E -> E - E, E -> E * E, E -> E / E
    else if (temp.size() >= 3 && temp[temp.size() - 3] == 'E' &&
             (temp[temp.size() - 2] == '+' || temp[temp.size() - 2] == '-' ||
              temp[temp.size() - 2] == '*' || temp[temp.size() - 2] == '/') &&
             temp[temp.size() - 1] == 'E')
    {
        char op = temp[temp.size() - 2];
        cout << "Reduce: E -> E" << op << "E" << endl;
        for (int i = 0; i < temp.size() - 3; i++)
            stk.push(temp[i]);
        stk.push('E'); // Push E for reduced expression
        return true;
    }
    // For E -> (E)
    else if (temp.size() >= 3 && temp[temp.size() - 3] == '(' &&
             temp[temp.size() - 2] == 'E' && temp[temp.size() - 1] == ')')
    {
        cout << "Reduce: E -> (E)" << endl;
        for (int i = 0; i < temp.size() - 3; i++)
            stk.push(temp[i]);
        stk.push('E'); // Push E for reduced expression
        return true;
    }
    // ----------------------------------------------------------------------------------------------
    // Restore stack if no reduction
    for (char c : temp)
        stk.push(c);
    return false;
}

// Operator precedence parser
void parse(string input)
{
    input += '$'; // Append end marker
    stack<char> stk;
    stk.push('$'); // Initialize stack with $

    int i = 0; // Input pointer
    int step_number = 0;
    while (i < input.length())
    {
        string currentToken;
        char current;

        // Extract current token without advancing pointer
        if (isdigit(input[i]))
        {
            while (i < input.length() && isdigit(input[i]))
            {
                currentToken += input[i];
                i++; // Temporarily advance to collect digits
            }
            current = 'n';              // Represent number as 'n'
            i -= currentToken.length(); // Rewind pointer
        }
        else
        {
            current = input[i];
            currentToken = current;
        }

        // Print stack and input buffer before action

        cout << endl
             << "Step: " << endl;
        printStack(stk);
        printInputBuffer(input, i);

        char top = stk.top();
        int row = getIndex(top);
        int col = getIndex(current);

        // If top is E, use second-top for precedence
        if (row == -1) // Top is E
        {
            row = getIndex(getSecondTop(stk));
        }

        // Check precedence
        int action = (row >= 0 && col >= 0) ? precedence[row][col] : -1;

        if (action == 1)
        { // Shift
            cout << "Shift: " << currentToken << endl;
            stk.push(current);
            printStack(stk);                                                           // Print stack after shift
            printInputBuffer(input, i + (current == 'n' ? currentToken.length() : 1)); // Print input after shift
            if (current == 'n')
                i += currentToken.length(); // Advance pointer for number
            else
                i++; // Advance pointer for single character
        }
        else if (action == -1)
        { // Reduce
            if (!tryReduce(stk))
            {
                cout << "Error: Invalid String (Cannot reduce stack)" << endl;
                printStack(stk);
                printInputBuffer(input, i);
                return;
            }
            // Print stack and input after reduction
            printStack(stk);
            printInputBuffer(input, i);
        }
        else if (action == 0)
        { // Accept
            cout << "Accept: Input is valid" << endl;
            printStack(stk);
            printInputBuffer(input, i);
            return;
        }
        else
        { // Invalid
            cout << "Error: Invalid string at " << currentToken << endl;
            printStack(stk);
            printInputBuffer(input, i);
            return;
        }
    }
}

int main()
{
    string input;
    cout << "Enter expression (e.g., 2+3*4, numbers and operators only): ";
    cin >> input;

    // Validate input
    for (char c : input)
    {
        if (!isdigit(c) && symbolIndex.find(c) == symbolIndex.end())
        {
            cout << "Invalid character: " << c << endl;
            return 1;
        }
    }

    parse(input);
    return 0;
}