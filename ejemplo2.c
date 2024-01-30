/* Microchip Technology Inc. and its subsidiaries.  You may use this software
 * and any derivatives exclusively with Microchip products.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
 * TERMS.
 */

/*MPLABX 2.45
 *XC32 v2.50
 *DFP 1.3.231
 *
 * rev1             Denny Hopp
 *                  1/28/21
 *                  - everything working with no dynamic memory allocation
 * rev2             Denny Hopp
 *                  1/28/21
 *                  - comment changes
 *                  - check added for NULL return on pointer search
 * rev3             Denny Hopp
 *                  2/1/21
 *                  - locationIsAvailable is cleared when a newNode is found
 * rev4             Denny Hopp
 *                  6/2/21
 *                  - comment corrections
 *                  - search through list starts at array index 0
 *

*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define TOTAL_NODES 25

// Create the node structure for each element in the linked list
struct Node
{
    uint8_t locationIsAvailable;
    uint32_t __attribute__((packed)) val;
    struct __attribute__((packed)) Node *nextPtr;
};

// reserve memory for 25 nodes.  This array is only used for memory allocation.
struct Node listReservedMemory[TOTAL_NODES];

// Create two pointers that will be used to search the linked list and add or delete nodes
struct Node *currentPointer;
struct Node *previousPointer;

// create the newNode that will be used to place data within the list
struct Node *newNode;

// create node designation for the first node in the list
struct Node *headNode;

// create a variable that will be used as a flag to denote that the search found a node that
//  we were looking for
uint8_t nodeFound;

void main(void)
{
    uint8_t i;

    // after establishing the pointers in the array, the array will no longer be used for node placement or
    //  deletion.  Everything moving forward after the list setup is using linked list parameters. The only time
    //  that the array will be used is to set the start location for searches.

    // establish the linked list search pointer
    currentPointer = &listReservedMemory[0];

    // initialize all list locations
    for (i = 0; i < TOTAL_NODES; i++)
    {
        currentPointer->locationIsAvailable = 1;
        currentPointer->val = 0;
        currentPointer->nextPtr = NULL;
        currentPointer++;
    }

    // initialize some nodes to demonstrate the search
    currentPointer = &listReservedMemory[0];
    headNode = &listReservedMemory[0];

    headNode->val = 2;
    headNode->locationIsAvailable = 0;
    headNode->nextPtr = ++currentPointer;

    currentPointer->val = 3;
    currentPointer->locationIsAvailable = 0;
    currentPointer->nextPtr = ++currentPointer;

    currentPointer->val = 5;
    currentPointer->locationIsAvailable = 0;
    currentPointer->nextPtr = NULL;

    newNode = NULL;

    //-------------------------------------------------------
    // [2] -> [3] -> [5] -> Null
    //-------------------------------------------------------

    // find the first unused location in the list to use as the newNode
    //  Search through up to TOTAL_NODES of nodes and will always be located in the
    //  allocated array memory
    currentPointer = &listReservedMemory[0];

    printf("--- Lista enlazada inicial ---\n");
    for (i = 0; i < TOTAL_NODES; i++)
    {
        //-------------------------------------------------------
        printf("[%d] -> ",currentPointer->val);
        //-------------------------------------------------------
        if (currentPointer->locationIsAvailable)
        {
            newNode = currentPointer;
            newNode->locationIsAvailable = 0;

            printf("NULL\n");

            break;
        }

        currentPointer++;
    }

    // only place the newNode if a valid pointer was returned from the search
    if (newNode != NULL)
    {
        // create a new node with a value of 4.  It is not placed in the list yet.
        newNode->val = 4;

        // add the node in numerical order in the linked list.  Initialize the node pointers.
        currentPointer = headNode;
        previousPointer = headNode;
        nodeFound = 0;

        // step through the linked list and search for the position to place the newNode value of 4
        //  so that the list stays in numerical order.

        // the algorithm looks at the value in the location of the currentPointer and compares it
        //  to the value in newNode.  If it is less than the value, the iteration of this search stops.
        //  previousPointer is set equal to currentPointer and currentPointer is set equal to the
        //  location pointed to by the nextPtr value in that location.  So this algorithm just steps
        //  through the list.  If the currentPointer value is greater than the newNode value, then the
        //  newNode value will be placed before the currentPointer.  Since previousPointer points to the
        //  location before the currentPointer, the nextPtr value of the previousPointer is set equal
        //  to the newNode pointer.  The newNode pointer is set to point to the currentPointer location.

        // For every iteration of the loop, the previousPointer is set equal to the currentPointer and
        //  the currentPointer is incremented.  So there are pointers to the current node and previous
        //  node at all times.

        // If the value to be placed is less than the first location in the list, the newNode pointer
        //  is set to be the headPointer.  If the value to be placed is greater than the last location,
        //  this search will fall through without placing the node in the list.  nodeFound is used to
        //  signify that a node was placed.  If this value is 0, then the previousPointer is set to point
        //  to the newNode and the newNode pointer is set to NULL.
        while (previousPointer->nextPtr != NULL)
        {
            if ((currentPointer->val) > (newNode->val))
            {
                nodeFound = 1;
                newNode->nextPtr = currentPointer;

                if (currentPointer != headNode)
                    previousPointer->nextPtr = newNode;

                else
                    headNode = newNode;

                break;
            }

            previousPointer = currentPointer;
            currentPointer = currentPointer->nextPtr;
        }

        // Check if a node was found.  If not, put the value at the end of the list
        if (!nodeFound)
        {
            previousPointer->nextPtr = newNode;
            newNode->nextPtr = NULL;
            nodeFound = 0;
        }

        nodeFound = 0;
    }

    // Delete the node with a value of 3.
    // This search is very similar to the previous search.  When the node value is found, the
    //  previousPointer is set to point to the value pointed by currentPointer.  Then the node
    //  value is set to 0 and the locationIsAvailable is set to 1.
    //  If the node to be deleted is the head node, then the headNode is set equal to the location
    //  pointed to by the current head node.  If the location is pointing to NULL, then it must be
    //  the last node, so the previousPointer is set to point to NULL since it will now be the last node.
    currentPointer = headNode;
    previousPointer = headNode;

    while (previousPointer->nextPtr != NULL)
    {
        if ((currentPointer->val) == 3)
        {
            if (currentPointer == headNode)
            {
                headNode = currentPointer->nextPtr;
            }

            else if (currentPointer->nextPtr == NULL)
            {
                previousPointer->nextPtr = NULL;
            }

            else
            {
                previousPointer->nextPtr = currentPointer->nextPtr;
            }

            currentPointer->locationIsAvailable = 1;
            currentPointer->nextPtr = NULL;
            break;
        }

        previousPointer = currentPointer;
        currentPointer = currentPointer->nextPtr;
    }

    /*
    while (1)
        ;
    */
}
