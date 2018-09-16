#pragma once


//****************************************************************************
//****************************************************************************
//****************************************************************************

#include "ccBlockPool.h"

//****************************************************************************
//****************************************************************************
//****************************************************************************

namespace CC
{

//****************************************************************************
//****************************************************************************
//****************************************************************************
// This is a base class for tree nodes.
//   
// It supports recursive data structures that are based on tree nodes and 
// collections of tree nodes. Nodes are attached to other nodes to form 
// collections that have the structure of trees.
//
// Nodes are elements of node collections, node collections contain nodes.
//   
// Nodes maintain pointers to neighboring nodes, similar to linked lists.
// There are methods provided that can be used to attach nodes to other
// nodes.
// 
// Node collections are ordered in two ways. First, any given node can
// havea node before it or a node after it. Second, any node can have a
// parent node that it belongs to above it, or it can have some number of
// child nodes that belong to it below it.
//   
//******************************************************************************
//******************************************************************************
//******************************************************************************

class TreeNode
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Memebers.

   // Blockhandle, used for block pool memory management.
   BlockHandle mBlockHandle;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Memebers.
   // Node pointers, they point to other neighboring nodes to which a node is
   // attached, If a pointer is null then there is no associated neighbor. The
   // child nodes of a node are managed via a linked list that starts at the 
   // first child node and ends at the last child node. The linked list can be
   // traversed via the before and after pointers of the child nodes.


   // The parent node of this node
   BlockHandle mParentNodeH;

   // The node before this node
   BlockHandle mBeforeNodeH;

   // The node after this node
   BlockHandle mAfterNodeH;

   // The first child node of this node
   BlockHandle mFirstChildNodeH;

   // The last child node of this node
   BlockHandle mLastChildNodeH;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Memebers.
   // This is a pointer to the closest ancestor of this node that has a node
   // after it. It is used by transmit and receive queues to deconstruct and
   // reconstruct tree node structures that are transmitted over a 
   // communications channel.
   //
   // This variable should be considered as a sort of temporary variable.
   // It is changed by various actions done by node queues. It should not
   // be used as a node level and it is not set by the attachment methods.
   // It is set by a transmit queue prior to transmission and is used
   // by a receive queue to reconstruct node structures.
   //
   // Use of this variable is mainly via the getNextNode visitor call,
   // which assumes a tree node structure traversal in construction order.

   BlockHandle mAncestorWithAfterH;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Memebers.
   // Node identifier.

   int mIdentifier;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Memebers.
   // Node flags, these encode relationship's that this node has with other 
   // nodes, within its tree structure. These flags all default to false.

   typedef union TreeNodeTxFlags
   {
      // Flags
      struct
      {
         // This is true if the node is the last in a sequence of a 
         // transmitted node structure
         bool mIsLastInStructure : 1;
      };
      // Binary value
      unsigned char mValue;
   } TreeNodeTxFlags;

   // These are the flags
   TreeNodeTxFlags mTreeNodeTxFlags;

   // This is the node attachment level. It is used by transmit and receive
   // queues to deconstruct and reconstruct tree node structures that are
   // transmitted over a communications channel.
   //
   // This variable should be considered as a sort of temporary variable.
   // It is changed by various actions done by node queues. It should not
   // be used as a node level and it is not set by the attachment methods.
   // It is set by a transmit queue prior to transmission and is used
   // by a receive queue to reconstruct node structures.
   //
   // Use of this variable is mainly via the getNextNode visitor call,
   // which assumes a tree node structure traversal in construction order.
   int mTxAttachLevel;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.
   // Memory management, constructors and destructors.

   // Allocate or deallocate a block from the block pool.
   static void* operator new      (size_t sz);
   static void  operator delete   (void* ptr);

   // Constructors.
   TreeNode();
   TreeNode(int aIdentifier);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.
   // These attach an object node to this subject node, as a child node

   // Attach an object node to the first child of this subject node, before it.
   // The object node becomes this subject node's first child.
   void attachBeforeFirstChild (TreeNode* aObjectNodeP);

   // Attach an object node to the last child of this subject node, after it.
   // The object node becomes this subject node's last child.
   void attachAfterLastChild (TreeNode* aObjectNodeP);
   // Attach as last in structure
   void attachAfterLastChildAsLastInStructure (TreeNode* aObjectNodeP);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.
   // These detach attached object nodes from this parent subject node.

   // Detach the first child node of this parent subject node. Return a
   // pointer to the detached child node.
   TreeNode* detachFirstChild ();

   // Detach the last child node of this parent subject node. Return a
   // pointer to the detached child node.
   TreeNode* detachLastChild ();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.
   // These attach an object node to this subject node.

   // Attach an object node to this subject node, before it
   void attachBefore (TreeNode* aObjectNodeP);

   // Attach an object node to this subject node, after it
   void attachAfter (TreeNode* aObjectNodeP);

   // Detach this subject node from all of the nodes to which it is attached.
   // This is used when a node is destroyed.
   void detachFromAll();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.
   // Handle to pointer conversions.

   static TreeNode* ptr(BlockHandle aBlockHandle);

   TreeNode* ptrParentNode()        { return ptr(mParentNodeH); }
   TreeNode* ptrBeforeNode()        { return ptr(mBeforeNodeH); }
   TreeNode* ptrAfterNode()         { return ptr(mAfterNodeH); }
   TreeNode* ptrFirstChildNode()    { return ptr(mFirstChildNodeH); }
   TreeNode* ptrLastChildNode()     { return ptr(mLastChildNodeH); }
   TreeNode* ptrAncestorWithAfter() { return ptr(mAncestorWithAfterH); }
};

//****************************************************************************
//****************************************************************************
//****************************************************************************
}//namespace

#if 0
//****************************************************************************
//****************************************************************************
//****************************************************************************
// Node pointers
/*

                                   +-----------+
         +------------------------>| parent    |<------------------------+
         |                         +-----------+                         |
         |                               ^                               |
         |                               |                               |
         |                               |                               |
   +-----------+                   +-----------+                   +-----------+
   | before    |<----------------->| subject   |<----------------->| after     |
   +-----------+                   +-----------+                   +-----------+
   before->mParentNode = parent    subject->mParentNode = parent   after->mParentNode = parent
   before->mAfterNode  = subject   subject->mBeforeNode = before   after->mBeforeNode = subject       
                                   subject->mAfterNode  = after

                                   parent->mFirstChildNode = first
                                   parent->mLastChildNode  = last
                                   +-----------+
         +-------------------------| parent    |-------------------------+
         |                         +-----------+                         |
         |                                                               |
         |                                                               |
         v                                                               v
   +-----------+                   +-----------+                   +-----------+
   | first     |<------...-------->| nodes     |<-------...------->| last      |
   +-----------+                   +-----------+                   +-----------+
   first->mParentNode = parent     node->mParentNode = parent      last->mParentNode = parent
*/

/*
TreeNode is a class that supports recursive data structures.

TreeNode is the basis for data structures that are based on trees of nodes.
It is similar to linked lists.

The tree data structures are collections of tree node elements. These data
structures are entirely analogous to two constructs: sets and file system
directory structures. 

The TreeNode class provides pointers to other nodes and it provides
methods that allow attaching other nodes to a given node.

Sets contain elements. The elements can be other sets or they can be what 
are known as ur-elements, elements that are not sets.

File directories contain other file directories or they contain files,
which are not directories.

Here are some rules which apply to sets and file directories:

Sets cannot contain themselves as elements, recursively or otherwise.

File directories cannot contain themselves as directories or files, 
recursively or otherwise.

A set cannot contain two elements which are identical.

A file directory cannot contain two files or directories which are identical.

These rules are not enforced by the TreeNode attach methods.

*/

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Attach an object node to a subject node, before it

/*
   CASE1 BEFORE, there is no node before the subject node

   parent->mFirstChildNode = subject
   +-----------+
   | parent    |
   +-----------+
      |     ^
      |     |
      v     |
   +-----------+
   | subject   |
   +-----------+
   subject->mParentNode = parent
   subject->mBeforeNode = 0

   CASE1 AFTER, there is no node before the subject node

   parent->mFirstChildNode = subject
   +-----------+
   | parent    |<-----------------------+
   +-----------+                        |
      |    ^                            |
      |    |                            |
      v    |                            |   
   +-----------+                   +-----------+
   | object    |<----------------->| subject   |       
   +-----------+                   +-----------+
   object->mParentNode = parent   subject->mBeforeNode = object
   object->mBeforeNode = 0         
   object->mAfterNode  = subject   

   CASE2 BEFORE, there is a node before the subject node

   +-----------+                   +-----------+
   | before    |<----------------->| subject   |       
   +-----------+                   +-----------+
   before->mAfterNode = subject     subject->mBeforeNode = before       

   CASE2 AFTER, there is a node before the subject node

   +-----------+                   +-----------+                   +-----------+
   | before    |<----------------->| object    |<----------------->| subject   |
   +-----------+                   +-----------+                   +-----------+
   before->mAfterNode = object    object->mBeforeNode = before     subject->mBeforeNode = object
                                  object->mAfterNode  = subject
                                  object->mParentNode = subject->mParentNode
*/        
//****************************************************************************
//****************************************************************************
//****************************************************************************
// Attach an object node to a subject node, after it

/*
   CASE1 BEFORE, there is no node after the subject node

   parent->mLastChildNode = subject
   +-----------+
   | parent    |
   +-----------+
      |     ^
      |     |
      v     |
   +-----------+
   | subject   |
   +-----------+
   subject->mParentNode = parent
   subject->mAfterNode  = 0

   CASE1 AFTER, there is no node after the subject node

   parent->mLastChildNode = object
   +-----------+
   | parent    |<-----------------------+
   +-----------+                        |
      |    ^                            |
      |    |                            |
      v    |                            |   
   +-----------+                   +-----------+
   | subject   |<----------------->| object    |
   +-----------+                   +-----------+
   subject->mAfterNode = object    object->mBeforeNode = subject 
                                   object->mAfterNode  = 0
                                   object->mParentNode = parent

   CASE2 BEFORE, there is a node after the subject node

   +-----------+                   +-----------+
   | subject   |<----------------->| after     |       
   +-----------+                   +-----------+
   subject->mAfterNode = after      after->mBeforeNode = subject   

   CASE2 AFTER, there is a node after the subject node

   +-----------+                   +-----------+                   +-----------+
   | subject   |<----------------->| object    |<----------------->| after     |
   +-----------+                   +-----------+                   +-----------+
   subject->mAfterNode = object    object->mBeforeNode = subject   after->mBeforeNode = object       
                                   object->mAfterNode  = after
                                   object->mParentNode = subject->mParentNode
*/
//****************************************************************************
//****************************************************************************
//****************************************************************************
// Detach this object node from its parent and before and after nodes

/*
   CASE1 BEFORE, there is no node before the object node

   parent->mFirstChildNode = object
   +-----------+
   | parent    |<-----------------------+
   +-----------+                        |
      |    ^                            |
      |    |                            |
      v    |                            |   
   +-----------+                   +-----------+
   | object    |<----------------->| after     |       
   +-----------+                   +-----------+
   object->mBeforeNode = 0         after->mBeforeNode = object
   object->mAfterNode  = after   

   CASE1 AFTER, there is no node before the object node

   parent->mFirstChildNode = after
   +-----------+
   | parent    |
   +-----------+
      |     ^
      |     |
      v     |
   +-----------+
   | after     |
   +-----------+
   after->mBeforeNode = 0

   CASE2 BEFORE, there is a node before the object node

   +-----------+                   +-----------+                   +-----------+
   | before    |<----------------->| object    |<----------------->| after     |
   +-----------+                   +-----------+                   +-----------+
   before->mAfterNode = object     object->mBeforeNode = before    after->mBeforeNode = object       
                                   object->mAfterNode  = after

   CASE2 AFTER, there is a node before the object node

   +-----------+                   +-----------+
   | before    |<----------------->| after     |       
   +-----------+                   +-----------+
   subject->mAfterNode = after      after->mBeforeNode = before

   CASE3 BEFORE, there is no node after the object node

   parent->mLastChildNode = object
   +-----------+
   | parent    |<-----------------------+
   +-----------+                        |
      |    ^                            |
      |    |                            |
      v    |                            |   
   +-----------+                   +-----------+
   | before    |<----------------->| object    |       
   +-----------+                   +-----------+
   before->mAfterNode = object    object->mBeforeNode = before        
                                   object->mAfterNode  = 0   

   CASE3 AFTER, there is no node after the object node

   parent->mLastChildNode = before
   +-----------+
   | parent    |
   +-----------+
      |     ^
      |     |
      v     |
   +-----------+
   | before    |
   +-----------+
   after->mAfterNode = 0

   CASE4 BEFORE, there is a node after the object node

   +-----------+                   +-----------+                   +-----------+
   | before    |<----------------->| object    |<----------------->| after     |
   +-----------+                   +-----------+                   +-----------+
   before->mAfterNode = object    object->mBeforeNode = before    after->mBeforeNode = object       
                                   object->mAfterNode  = after

   CASE4 AFTER, there is a node before the object node

   +-----------+                   +-----------+
   | before    |<----------------->| after     |       
   +-----------+                   +-----------+
   before->mAfterNode = after      after->mBeforeNode = before       

*/        
#endif