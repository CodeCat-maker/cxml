# CXML ![](https://img.shields.io/badge/license-MIT-blue)![](https://img.shields.io/badge/language-C++-blue)![](https://img.shields.io/badge/version-2.0-blue)

A lightweight C++ XML parser with support for XPath expressions.

- [CXML](#cxml-----https---imgshieldsio-badge-license-mit-blue-----https---imgshieldsio-badge-language-c--blue-----https---imgshieldsio-badge-version-20-blue-)
  - [Introduction](#introduction)
- [Usage](#usage)
  - [Installation](#installation)
  - [Demo](#demo)
  - [Supported XPath Syntax](#supported-xpath-syntax)
  - [CMakeLists](#cmakelists)
  - [Build](#build)
  - [Example Output](#example-output)
- [Design](#design)
  - [1. Core Logic](#1-core-logic)
    - [1. Input Handling](#1-input-handling)
    - [2. Parsing](#2-parsing)
    - [3. Building the XML Tree](#3-building-the-xml-tree)
    - [4. XPath Evaluation](#4-xpath-evaluation)
      - [Approach](#approach)
        - [Constants](#constants)
        - [Queue Construction with Two-Pointer Parsing](#queue-construction-with-two-pointer-parsing)
        - [Switch-Based Execution](#switch-based-execution)
        - [BFS Traversal](#bfs-traversal)
        - [DFS Traversal](#dfs-traversal)
    - [5. Data Structures](#5-data-structures)
    - [6. STL Containers](#6-stl-containers)
      - [Algorithms](#algorithms)
  - [2. Future Improvements](#2-future-improvements)
- [Contributions](#contributions)

<small><i><a href='http://ecotrust-canada.github.io/markdown-toc/'>Table of contents generated with markdown-toc</a></i></small>

---

## Introduction

**CXML** is a simple and efficient XML parser written in C++ with built-in support for XPath queries.  
The project combines fundamental C++ techniques with classic data structures to deliver a practical tool for XML data processing.

---

# Usage

## Installation

```bash
git clone https://github.com/CodeCat-maker/cxml.git
```

## Demo

```c++
#include "src/parser.hpp"
#include "src/xpath.hpp"
extern int CXML_PARSER_STATUS;
extern int XPATH_PARSE_STATUE;

int main() {
    using std::cout;
    using std::endl;
    clock_t start, end;
    start = clock();

    CXMLNode *root = parse_from_string(
        "<bookstore company=\"codecat\" boss=\"man\">"
        "  <book category=\"CHILDREN\">"
        "    <title>Harry Potter</title>"
        "    <author>J K.Rowlingk</author>"
        "    <year>2005</year><br>"
        "    <price>29.99 </price>"
        "  </book>"
        "  <book category=\"WEB\">"
        "    <title>Learning XML</title>"
        "    <author>Erik T.Ray</author>"
        "    <year>2003 </year>"
        "    <price>39.95 </price>"
        "  </book>"
        "</bookstore>"
    );

    if (CXML_PARSER_STATUS == CXML_SYNTAX_ERROR) {
        std::puts("> XML parsing failed");
        return 0;
    } else {
        std::puts("> XML parsing succeeded");
    }

    const CXMLNode_result *result1 =
        xpath("/bookstore/book[@category=CHILDREN]/@category//text()", root);
    const CXMLNode_result *result2 =
        xpath("/bookstore/book/title/../price/text()", root);

    if (XPATH_PARSE_STATUE == XPATH_SYNTAX_ERROR) {
        std::puts("> XPath parsing failed");
        return 0;
    } else {
        std::puts("> XPath parsing succeeded");
    }

    cout << "Example 1: " << result1->text << endl;
    cout << "Example 2: " << result2->text << endl;

    end = clock();
    cout << "\nExecution time: "
         << (double)(end - start) / CLOCKS_PER_SEC << " seconds";
    return 0;
}
```

## Supported XPath Syntax

- `/name` – Selects child element `name`
- `//name` – Selects descendant element `name`
- `/.` – Selects the current element
- `/..` – Selects the parent element
- `/name[@attr=value]` – Selects elements with attribute `attr=value`
- `/name[@attr]` – Selects elements with attribute `attr`
- `/name[n]` – Selects the nth occurrence of `name`
- `/text()` – Returns text content of the current element
- `/@attr` – Returns the value of attribute `attr`
- `//text()` – Returns all descendant text nodes
- `//@attr` – Returns all descendant attributes

## CMakeLists

```cmake
cmake_minimum_required(VERSION 3.13)

project(cxml)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(src)

add_executable(cxml main.cpp)

target_link_libraries(cxml CxmlFunction)
```

## Build

```bash
mkdir -p build
cd build
cmake ..
make -j
```

## Example Output

```
> XML parsing succeeded
> XPath parsing succeeded
Example 1: Harry Potter J K.Rowlingk 2005 29.99
Example 2: 29.99

Execution time: 0.000135 seconds
```

---

# Design

## 1. Core Logic

### 1. Input Handling
- From file
- From string

### 2. Parsing
- Parse element name
- Parse attributes
- Parse element text
- Construct the XML tree

### 3. Building the XML Tree
- **Stack-based construction**
  - Push `<tag>` on open
  - Pop on `</tag>`
  - Naturally maintains parent–child relationships
- **Recursive descent** for nested content

### 4. XPath Evaluation

#### Approach
Tokenize the expression with a two-pointer scan, enqueue operations, and evaluate in FIFO order.

##### Constants
- `get_parent_node` – `/..`
- `get_this_node` – `/.`
- `get_node_from_child_by_name` – `/name`
- `get_node_from_genera_by_name` – `//name`
- `get_node_by_array_and_name` – `/name[n]`
- `get_node_by_attr_and_name` – `/name[@attr]`
- `get_node_by_attrValue_and_name` – `/name[@attr=value]`
- `get_text_from_this` – `/text()`
- `get_texts_from_genera` – `//text()`
- `get_attr_from_this` – `/@attr`

##### Queue Construction with Two-Pointer Parsing
Split the XPath string into tokens and enqueue them.

##### Switch-Based Execution
Dequeue operations and dispatch to the corresponding handlers.

##### BFS Traversal
Collect text nodes from all descendants.

##### DFS Traversal
Search for the first matching descendant by name or predicate.

### 5. Data Structures
- Stack, linked list, tree, queue, pair/tuple

### 6. STL Containers
- `vector`, `map`, `string`, `pair`, `stack`, `queue`

#### Algorithms
- DFS, BFS, two-pointer parsing

---

## 2. Future Improvements
- HTML parsing support
- Extended XPath coverage
- Performance optimizations

---

# Contributions
Contributions are welcome—feel free to fork and open a PR.
