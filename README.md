# Minimalist Browser

This project implements a **minimalist browser manager** in C, simulating the internal navigation logic of a modern web browser. It was developed for the first assignment in the **Data Structures and Algorithms** course (SDA), focusing on correct and efficient use of **stacks** and **circular doubly linked lists**.

The system manages multiple tabs, each with individual navigation history using `backward` and `forward` stacks, and supports a series of commands that replicate user behavior in a simplified browser.

## Data Structures

The following types were implemented to reflect the browser architecture:

- **`page`**: Represents a web page with an `id`, a `url` (max 50 characters), and a dynamically allocated `description`.  
  → Used in navigation and history tracking.

- **`stacknode` / `stack`**: Implemented as a generic stack to manage the backward and forward navigation history inside each tab.

- **`tab`**: Holds a unique ID, a pointer to the current page, and two stacks (`backwardStack` and `forwardStack`) for tracking browsing history.  
  → Each tab is a node in the circular doubly linked list.

- **`listNode` / `tabsList`**: Nodes and structure for a circular doubly linked list with a sentinel, used to manage all open tabs in the browser.

- **`browser`**: Maintains a pointer to the current tab and the full tab list. Also contains `maxtabid`, incremented to assign unique IDs for new tabs.

- **`pages`**: Holds a dynamically allocated array of all predefined pages from input (except the implicit one).

## Command Implementation

- **`NEW_TAB`**  
  → `create_newtab()` adds a new tab to the list and moves the `current` pointer to it. Sets the default page.

- **`CLOSE`**  
  → `close_tab()` removes the current tab and updates the `current` pointer to the previous one (unless it's the default tab, which cannot be closed).

- **`OPEN <ID>`**  
  → `open_tab()` searches the list and switches the current tab if it exists.

- **`NEXT / PREV`**  
  → Traverse the list forward or backward to change the active tab.

- **`PAGE <ID>`**  
  → `open_page()` sets a new current page in the current tab. Moves the old page to `backwardStack` and clears `forwardStack`.

- **`BACKWARD`**  
  → If possible, moves to the last page in `backwardStack`, pushing the current page to `forwardStack`.

- **`FORWARD`**  
  → If possible, moves to the last page in `forwardStack`, pushing the current page to `backwardStack`.

- **`PRINT`**  
  → Displays all tab IDs starting from the current one (circular), followed by the current page's description.

- **`PRINT_HISTORY <ID>`**  
  → Shows the URL history of a specific tab in order: `forwardStack` (oldest to newest), current page, `backwardStack` (newest to oldest).

## Memory Management

- Generic creation and destruction functions were written for all structures.
- Full cleanup is ensured via:
  - `freeTab()`: Frees one tab’s content and stacks
  - `freeList()`: Frees the entire list of tabs
  - `freeBrowser()`: Frees the entire browser structure
- Memory is **entirely dynamically allocated** and **Valgrind-clean**.

## Files

- `main.c`: Data structures, parsing, and command handling
- `Makefile`: Compilation and cleaning
- `README.md`: This file
- `README` : Original README file for the assignment

## Build & Run

```bash
make          # Build the executable (outputs 'tema1')
./tema1       # Run with custom input files
./checker.sh  # Check with predefined input files
```

## Course Info

📚 Data Structures & Algorithms (SDA) — 2025
🧠 Task Score: 100/100
🔍 Valgrind Bonus: 20/20