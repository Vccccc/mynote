#### Basic wildcards
**Wildcard** | **Matches**
|:--------|:------------|
| ? | Any single character|
| * | Any string of characters|
| [set] | Any character in set |
| [!set] | Any character not in set |

#### Using the set construct wildcards
**Expression** | **Matches** |
|:------|:------------|
| [abc] | a, b, or c |
| [.,;] | Perioad, comma, or semicolon |
| [-_] | Dash or underscore |
| [a-c] | a, b, or c |
| [a-z] | All lowercase letters |
| [!0-9] | All non-digits |
| [0-9!] | All digits and exclamation point |
| [a-zA-Z] | All lower-and uppercase letters |
| [a-zA-Z0-9_-] | All letters, all digits, underscore, and dash |

#### Popular UNIX data filtering utilities
**Utility** | **Purpose** |
|:----------|:-----------|
| cat | Copy input to output |
| grep | Search for strings in the 
