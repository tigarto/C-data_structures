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
 *                  1/20/21
 *                  - original code
 * rev2             Denny Hopp
 *                  1/21/21
 *                  - node adding working for all cases
 *                  - nodel deletion working for non-head and tail nodes
 * rev3             Denny Hopp
 *                  1/21/21
 *                  - node delete is working for all cases
 * rev4             Denny Hopp
 *                  1/22/21
 *                  - comments added
 * rev5             Denny Hopp
 *                  1/22/21
 *                  - comment changes
 * rev6             Denny Hopp
 *                  5/28/2021
 *                  - added stddef.h
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// Create the node structure for each element in the linked list
struct Node
{
    int val;
    struct Node *nextPtr;
};

// Create two pointers that will be used to search the linked list and add or delete nodes
struct Node *currentPointer;
struct Node *previousPointer;

// create a variable that will be used as a flag to denote that the search found a node that
//  we were looking for
uint8_t nodeFound;

void main(void) {
    // Create structure pointer to four instanced of nodes in the linked list.  The only required
    //  nodes to be created are "headNode" to signify the first node in the linked list and "newNode"
    //  that will be used to manipulate the items in the list.  The "secondNode" and "thirdNode"
    //  are created just for demonstration purposes to allow for some initial created nodes and make
    //  it easier to understand the linked list concept.
    struct Node *headNode = NULL;
    struct Node *secondNode = NULL;
    struct Node *thirdNode = NULL;
    struct Node *newNode = NULL;

    // allocate 3 nodes in the heap
    headNode = (struct Node *)malloc(sizeof(struct Node));
    secondNode = (struct Node *)malloc(sizeof(struct Node));
    thirdNode = (struct Node *)malloc(sizeof(struct Node));

    // initialize all nodes
    headNode->val = 2;
    headNode->nextPtr = secondNode;
    secondNode->val = 3;
    secondNode->nextPtr = thirdNode;
    thirdNode->val = 5;
    thirdNode->nextPtr = NULL;

    //-------------------------------------------------------
    // [2] -> [3] -> [5] -> Null
    //-------------------------------------------------------

    // create a new node with a value of 4.  It is not placed in the list yet.
    newNode = (struct Node *)malloc(sizeof(struct Node));
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
    printf("--- Lista enlazada inicial ---\n");
    while (previousPointer->nextPtr != NULL)
    {
        //-------------------------------------------------------
        printf("[%d] -> ",currentPointer->val);
        //-------------------------------------------------------
        
        if ((currentPointer->val) > (newNode->val))
        {
            nodeFound = 1;
            newNode->nextPtr = currentPointer;

            if (currentPointer != headNode)
                previousPointer->nextPtr = newNode;

            else
                headNode = newNode;

            printf("NULL\n");
            break;
        }

        previousPointer = currentPointer;
        currentPointer = currentPointer->nextPtr;
    }

    // Check if a node was found.  If not, put the value at the end of the list
    printf("Verificando si un nodo se encuentra: %d\n", nodeFound);
    if (!nodeFound)
    {
        previousPointer->nextPtr = newNode;
        newNode->nextPtr = NULL;
        nodeFound = 0;
    }
    printf("Se agrego un nuevo nodo [%d] al final de la lista:\n", newNode->val);

    //-------------------------------------------------------
    // [2] -> [3] -> [5] -> [4] -> Null
    //-------------------------------------------------------

    nodeFound = 0;

    printf("Borrando el nodo [3] de la lista enlazada\n");


    // Delete the node with a value of 3.
    // This search is very similar to the previous search.  When the node value is found, the
    //  previousPointer is set to point to the value pointed by currentPointer.  Then the free( )
    //  function call is used to deallocate the memory that currentPointer was pointing to.  If the
    //  node to be deleted is the head node, then the headNode is set equal to the location pointed
    //  to by the current head node.  If the location is pointing to NULL, then it must be the last
    //  node, so the previousPointer is set to point to NULL since it will now be the last node.
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

            free(currentPointer);
            break;
        }

        previousPointer = currentPointer;
        currentPointer = currentPointer->nextPtr;
    }

    //-------------------------------------------------------
    // [2] -> [5] -> [4] -> Null
    //-------------------------------------------------------

    printf("Pendiente para hacer el uso del GDB\n");
    
    /*
    while (1)
        ;
    */
}
