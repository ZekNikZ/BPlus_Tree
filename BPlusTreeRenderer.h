/*
 * Author: Team Booth-Plus Tree
 * Assignment Title: CSI 3334 Group Project
 * Assignment Description: This program is a visualization tool for a B+ Tree.
 * Due Date: 12/9/2019
 * Date Created: 11/23/2019
 * Date Last Modified: 12/7/2019
 */

#ifndef RENDERER_H
#define RENDERER_H

// STL Imports
#include <iostream>
#include <sstream>

// Project Imports
#include "SDL_Plotter.h"
#include "BitmapImage.h"
#include "CharacterGraphics.h"
#include "Rectangle.h"
#include "Color.h"
#include "Arrow.h"

#include "BPlusTree.h"
/*
 * Renderer for a BPlusTree.
 */
template <typename T>
class BPlusTreeRenderer {
private:
    // Constants
    const int NODE_PADDING = 8;

    struct RendNode {
        /*
         * Description: RendNode constructor
         * Return: none
         * Precondition: node is a valid Node
         * Postcondition: a RendNode object is constructed
         */
        RendNode(Node<T> * node, int width = 0, int x = -1);

        Node<T> * node;
        int x;
        int width;
        vector<RendNode *> children;
        vector<int> valXs;
    };

    CharacterGraphics characters;

    /*
     * Description: draw a RendNode onto the screen
     * Return: none
     * Precondition: rendNode contains a valid Node in a valid tree
     * Postcondition: the RendNode is drawn to the screen at the specified
     *     position
     */
    void drawNode(SDL_Plotter & g, RendNode * rendNode, int x, int y);

    /*
     * Description: get the width of a Node without drawing it
     * Return: none
     * Precondition: rendNode contains a valid Node in a valid tree
     * Postcondition: the RendNode is drawn to the screen at the specified
     *     position
     */
    int getRenderedNodeWidth(Node<T> * node);

public:
    /*
     * Description: BPlusTreeRenderer constructor
     * Return: none
     * Precondition: the character image file exists
     * Postcondition: a BPlusTreeRenderer object is constructed
     *     position
     */
    BPlusTreeRenderer();

    /*
     * Description: draw a BPlusTree
     * Return: none
     * Precondition: tree is a properly-constructed tree
     * Postcondition: the RendNode is drawn to the center of the screen with the
     *     specified offset
     */
    void draw(SDL_Plotter & g, const BPlusTree<T> & tree, int x = 0, int y = 0);
};

/*
 * Description: RendNode constructor
 * Return: none
 * Precondition: node is a valid Node
 * Postcondition: a RendNode object is constructed
 */
template<typename T>
BPlusTreeRenderer<T>::RendNode::RendNode(Node<T> * node,
                                         int width, int x) :
        node(node), x(x), width(width) {
}

// Global spacing constants

/*
 * Description: draw a RendNode onto the screen
 * Return: none
 * Precondition: rendNode contains a valid Node in a valid tree
 * Postcondition: the RendNode is drawn to the screen at the specified
 *     position
 */
template<typename T>
void BPlusTreeRenderer<T>::drawNode(SDL_Plotter & g, RendNode * rendNode,
                                    int x, int y) {
    // Spacing constants
    static const int CHAR_WIDTH = characters.getCharWidth();
    static const int CHAR_HEIGHT = characters.getHeight();
    static const int CHAR_SPACING = characters.getSpacing();

    // Color constants
    static const Color DATA_OUTLINE(82, 169, 250);
    static const Color DATA_BACKGROUND(173, 216, 255);
    static const Color KEY_OUTLINE(68, 227, 142);
    static const Color KEY_BACKGROUND(173, 255, 187);

    // Helper variables
    stringstream ss;  // the stringstream to convert values to strings
    string s;         // the output string

    // Setup cell rectangle (fill)
    Rectangle cell(x, y, 0, 2 * NODE_PADDING + CHAR_HEIGHT, 1,
            rendNode->node->type == Node<T>::KEY ?
                    KEY_OUTLINE : DATA_OUTLINE, true,
            rendNode->node->type == Node<T>::KEY ?
                    KEY_BACKGROUND : DATA_BACKGROUND);

    // Setup entire node rectangle (outline)
    Rectangle nodeRect = cell;
    nodeRect.setFill(false);
    nodeRect.setOutline(2);

    // Get data of the node
    vector<T> data = rendNode->node->vals;

    // Draw each value
    for (auto datum : data) {
        // Convert value into a string
        ss.clear();
        ss << datum;
        ss >> s;

        // Compute width of the cell
        int width = 2 * NODE_PADDING + CHAR_WIDTH * s.length()
                    + CHAR_SPACING * (s.length() - 1);

        // Adjust rectangle sizes with this width
        cell.setWidth(width);
        nodeRect.stretchX(width - 1);

        // Draw the background of the cell
        cell.draw(g);

        // Draw the value
        characters.draw(g, cell.getX() + NODE_PADDING,
                        cell.getY() + NODE_PADDING, s);

        // Store the x position of the cell for drawing the arrows later
        rendNode->valXs.push_back(cell.getX());

        // Move the cell rectangle to the next location
        cell.shiftX(width - 1);
    }

    // Store the x position of the back of the cell for drawing the arrows later
    rendNode->valXs.push_back(cell.getX());

    // Draw node rectangle (outline)
    nodeRect.draw(g);
}

/*
 * Description: get the width of a Node without drawing it
 * Return: none
 * Precondition: rendNode contains a valid Node in a valid tree
 * Postcondition: the RendNode is drawn to the screen at the specified
 *     position
 */
template<typename T>
int BPlusTreeRenderer<T>::getRenderedNodeWidth(
        Node<T> * node) {
    // Spacing constants
    static const int CHAR_WIDTH = characters.getCharWidth();
    static const int CHAR_SPACING = characters.getSpacing();

    // Helper variables
    stringstream ss;    // the stringstream to convert values to strings
    string s;           // the output string
    int fullWidth = 0;  // the width of the node

    // Get data
    vector<T> data = node->vals;

    // Draw each value
    for (auto datum : data) {
        // Convert value to a string
        ss.clear();
        ss << datum;
        ss >> s;

        // Compute the width of this cell and add it to the fullWidth
        fullWidth += 2 * NODE_PADDING + CHAR_WIDTH * s.length()
                     + CHAR_SPACING * (s.length() - 1);
    }

    return fullWidth;
}

/*
 * Description: BPlusTreeRenderer constructor
 * Return: none
 * Precondition: the character image file exists
 * Postcondition: a BPlusTreeRenderer object is constructed
 *     position
 */
template<typename T>
BPlusTreeRenderer<T>::BPlusTreeRenderer() :
        characters("images/numbers.bmp", "0123456789") {
}

/*
 * Description: draw a BPlusTree
 * Return: none
 * Precondition: tree is a properly-constructed tree
 * Postcondition: the RendNode is drawn to the center of the screen with the
 *     specified offset
 */
template<typename T>
void BPlusTreeRenderer<T>::draw(SDL_Plotter & g, const BPlusTree<T> & tree, int x,
                             int y) {
    // Spacing constants
    static const int MIN_SPACE_BETWEEN_NODES = 15;
    static const int LEVEL_SEPARATION = 20;
    static const int CHAR_HEIGHT = characters.getHeight();

    // Color constants
    static const int ARROW_COLOR_NUM = 170;
    static const Color ARROW_COLOR(ARROW_COLOR_NUM, ARROW_COLOR_NUM, ARROW_COLOR_NUM);

    // If there is no tree to draw, stop here
    if (tree.size == 0 || tree.root == nullptr) return;

    // Levels vector, this will hold all of the RendNodes with their connections
    vector<vector<RendNode *>> levels;  // each level of the tree

    // Add the root level
    levels.push_back(vector<RendNode *>());
    levels.front().push_back(
            new RendNode(tree.root, getRenderedNodeWidth(tree.root)));

    // Make level vectors
    RendNode * topNode = levels.back().front();
    while (topNode->node->type != Node<T>::DATA) {
        // Create next level vector
        levels.push_back(vector<RendNode *>());

        // Add nodes to next level
        for (RendNode * rendNode : levels[levels.size() - 2]) {
            for (auto child : rendNode->node->ptrs) {
                // Create a new RendNode for this child
                levels.back().push_back(
                        new RendNode(child, getRenderedNodeWidth(child)));

                // Mark this node as a child of its parent
                rendNode->children.push_back(levels.back().back());
            }
        }

        // Update topNode
        topNode = levels.back().front();
    }

    // Compute width of bottom level
    int bottomLevelWidth = (levels.back().size() - 1) * MIN_SPACE_BETWEEN_NODES;
    for (RendNode * rendNode : levels.back()) {
        bottomLevelWidth += rendNode->width;
    }

    // Compute row height
    int rowHeight = (2 * NODE_PADDING + CHAR_HEIGHT);

    // Compute total tree height
    int totalTreeHeight = rowHeight * levels.size()
            + LEVEL_SEPARATION * (levels.size() - 1);

    // Compute starting x and y location for drawing
    int xDraw = (g.getCol() - bottomLevelWidth) / 2 + x;
    int yDraw = (g.getRow() - totalTreeHeight) / 2 + totalTreeHeight - rowHeight
            + y;

    // Draw bottom row
    for (typename vector<RendNode *>::iterator it = levels.back().begin(); it != levels.back().end(); ++it) {
        RendNode * thisNode = *it;

        // Draw current node
        drawNode(g, *it, xDraw, yDraw);

        // Store the x position of where it was drawn
        thisNode->x = xDraw;

        // Draw an arrow to the next node, if applicable
        if (it + 1 != levels.back().end()) {
            // Move the drawing x location to the next node
            xDraw += thisNode->width + MIN_SPACE_BETWEEN_NODES;

            // Draw the arrow
            if (thisNode->node->ptrs.size() == 1) {
                Arrow::drawArrow(g, xDraw - MIN_SPACE_BETWEEN_NODES,
                                 yDraw + rowHeight / 2, xDraw,
                                 yDraw + rowHeight / 2);
            }
        }
    }

    // Draw other rows
    auto it = levels.rbegin();  // iterator to go through the vector of vectors
    for (++it; it != levels.rend(); ++it) {
        // Move draw Y up to the next drawing level
        yDraw -= LEVEL_SEPARATION + rowHeight;

        // Draw each node on this level
        for (RendNode * rendNode : *it) {
            // Compute the x location of the current node
            xDraw = (rendNode->children.front()->x
                    + rendNode->children.back()->x
                    + rendNode->children.back()->width)
                    / 2
                    - rendNode->width / 2;

            // Draw the current node
            drawNode(g, rendNode, xDraw, yDraw);

            // Store the x position of where it was drawn
            rendNode->x = xDraw;

            // Draw arrows to children
            for (int i = 0; i < rendNode->children.size(); ++i) {
                Arrow::drawArrow(g, rendNode->valXs[i], yDraw + rowHeight,
                                 rendNode->children[i]->x,
                                 yDraw + LEVEL_SEPARATION + rowHeight);
            }
        }
    }

    // Clean up dynamically-allocated memory
    for (auto level : levels) {
        for (auto rendNode : level) {
            delete rendNode;
        }
    }
}

#endif // RENDERER_H
