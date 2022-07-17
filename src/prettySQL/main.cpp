#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include <list>
#include <string>
#include <algorithm>

namespace
{

/**
 * Reads all data from stdin and returns it as a string
 *
 * @return stdin data
 */
std::string readStdin()
{
    // Get data
    size_t len = 0;
    std::string query;
    char data[2048] = {0};
    while ((len = fread(data, 1, sizeof(data), stdin)) > 0)
        query.append(data, static_cast<size_t>(len));

    return query;
}

/**
 * Prints the whitespace for an indention
 *
 * @param depth Indention depth
 */
void printIndent(unsigned int depth)
{
    printf("\n");
    for (unsigned int ui = 0; ui < depth; ui++)
        printf("    ");
}

/**
 * Case insensitive try to match the next part of a string
 *
 * @param query The full string
 * @param pos The start position in the string to match
 * @param strings The possible strings to match against
 *
 * @return true=Match, false=No match
 */
bool match(
    const std::string &query,
    size_t pos,
    const std::list<const char *> &strings)
{
    for (const char *str : strings)
    {
        size_t len = strlen(str);
        if (pos + len > query.length())
            continue;

        std::string substr = query.substr(pos, len);
        std::transform(substr.begin(), substr.end(), substr.begin(), ::toupper);
        if (substr == str)
            return true;
    }

    return false;
}

/**
 * Canonicalize whitespace in a query (remove redundant, make everything a single space)
 *
 * @param query The query to canonicalize
 */
void canonicalWhitespace(
    std::string &query)
{
    // Remove extra whitespace
    size_t pos = 0;
    bool inString = false;
    bool prevSpace = false;
    while (pos < query.length())
    {
        // Handle strings
        if (query[pos] == '\'')
        {
            pos++;
            prevSpace = false;
            if (!inString)
                inString = true;
            else if (query[pos] == '\'')
                pos++;
            else
                inString = false;
            continue;
        }

        // Don't mess with whitespace in strings
        if (inString)
        {
            pos++;
            continue;
        }

        const bool curSpace = isspace(query[pos]);
        // Remove duplicate whitespace
        if (curSpace && prevSpace)
        {
            query.erase(pos, 1);
        }
        // Remove duplicate whitespace and canonicalize to space
        else if (curSpace)
        {
            query[pos++] = ' ';
            prevSpace = curSpace;
        }
        // Not whitespace
        else
        {
            ++pos;
            prevSpace = false;
        }
    }
}

/**
 * Pretty the SQL query in the pretty human readable format
 *
 * @param query The query to print
 */
void prettyPrint(
    const std::string &query)
{
    // Print
    size_t pos = 0;
    bool inString = false;
    bool comeAndGone = true;
    unsigned int nested = 0;
    std::list<unsigned int> depths;
    while (pos < query.length())
    {
        // Handle strings
        if (query[pos] == '\'')
        {
            printf("%c", query[pos++]);
            if (!inString)
                inString = true;
            else if (query[pos] == '\'')
                printf("%c", query[pos++]);
            else
                inString = false;
            continue;
        }

        // Ignore characters in strings
        if (inString)
        {
            // Do nothing
        }
        // Nesting into a paren
        else if (query[pos] == '(')
        {
            nested++;
        }
        // Possibly going down a depth
        else if (query[pos] == ')' && nested > 0)
        {
            // There is a depth to go down, and it is associated with this paren
            if (!depths.empty() && depths.back() == nested)
            {
                comeAndGone = true;
                depths.pop_back();
                printIndent(depths.empty() ? 0 : depths.back());
            }
            nested--;
        }
        // Going up an indention
        else if (match(query, pos, {"FROM", "WHERE", "SELECT", "WITH", "ORDER BY", "LIMIT"}))
        {
            // We need to have traversed a paren to go up another depth
            if (depths.empty() || comeAndGone || (depths.back() != nested))
            {
                comeAndGone = false;
                depths.emplace_back(nested);
            }
            printIndent(depths.back());
        }
        // Union we will dislay on its own line
        else if (match(query, pos, {"UNION"}))
        {
            constexpr const size_t UNION_LEN = sizeof("UNION") - sizeof('\0');
            printIndent(depths.empty() ? 1 : (depths.back() + 1));
            printf("%s", query.substr(pos, UNION_LEN).c_str());
            pos += UNION_LEN;
            continue;
        }

        printf("%c", query[pos++]);
    }

    printf("\n");

}

/**
 * Run the test cases
 */
void tests()
{
    constexpr const char *TEST1 =
        "WITH FirstCTE AS"
        "   (SELECT Id EmployeeId, Name, ManagerId, 'Some text ghw''s   birthday WITH() SELECT(BLAH)' AS    mytest"
        "    FROM dbo.Employees WITH(NOLOCK) "
        "    WHERE ManagerId is NULL),   SecondCTE AS"
         "   (SELECT E.Id EmployeeId, E.Name, E.ManagerId, "
        "            FCTE.Name ManagerName"
        "     FROM dbo.Employees E WITH(NOLOCK)"
        "        INNER JOIN FirstCTE FCTE"
        "         ON E.ManagerId = FCTE.EmployeeId)"
        "SELECT * FROM SecondCTE";
    prettyPrint(TEST1);

    constexpr const char *TEST2 =
        "SELECT COUNT(id) FROM (SELECT * FROM (SELECT table.id, table.id2 AS second_id FROM table WHERE (table.column=12) AND (table.other>'2022-01-01') AND ((table.something<>'Test'))) AS leftTable JOIN (SELECT DISTINCT object_id FROM secondtable WHERE type=111 AND none_type<>123 AND something IN (1,2,3,4,5)) AS rightTable ON leftTable.id=rightTable.object_id) AS FinalQuery;";
    prettyPrint(TEST2);

    constexpr const char *TEST3 =
        "SELECT id FROM firsttable WHERE blah=1 UNION ALL SELECT id FROM secondtable WHERE blahblah=2 ORDER BY id";
    prettyPrint(TEST3);
}

}

/**
 * Reads long SQL strings from stdin and prints them with extra
 * whitespace to stdout for readability
 *
 * @param argc stdargs count
 * @param argv stdargs values
 *
 * @return 0 (Success), 1 on usage help
 */
int main(int argc, const char *argv[])
{
    if (argc > 1)
    {
        // Run tests
        if (!strcmp(argv[1], "test"))
        {
            tests();
            return 0;
        }
        // Show usage
        else
        {
            fprintf(stderr, "Parse an SQL statement into a more human readable format.\n");
            fprintf(stderr, "Usage: %s [test]\n", argv[0]);
            return 1;
        }
    }

    std::string query = readStdin();
    canonicalWhitespace(query);
    prettyPrint(query);

    return 0;
}
