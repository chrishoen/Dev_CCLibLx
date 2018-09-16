
#include "stdafx.h"

#include "ccTreeNodeVisitor.h"

namespace CC
{

//****************************************************************************
//****************************************************************************
//****************************************************************************

void visitAllNodesCO(
   TreeNode*         aSubjectNodeP,
   RecursiveAnchor*  aRecursiveAnchor,
   NodeVisitorCall*  aNodeVisitorCall)
{
   // Visit the subject node
   (*aNodeVisitorCall)(aSubjectNodeP, aRecursiveAnchor);

   // Visit all nodes below the subject node
   visitAllNodesBelowCO(
      aSubjectNodeP,
      aRecursiveAnchor,
      aNodeVisitorCall);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************

void visitAllNodesDO(
   TreeNode*         aSubjectNodeP,
   RecursiveAnchor*  aRecursiveAnchor,
   NodeVisitorCall*  aNodeVisitorCall)
{
   // Visit all nodes below the subject node
   visitAllNodesBelowDO(
      aSubjectNodeP,
      aRecursiveAnchor,
      aNodeVisitorCall);

   // Visit the subject node
   (*aNodeVisitorCall)(aSubjectNodeP, aRecursiveAnchor);

}

//****************************************************************************
//****************************************************************************
//****************************************************************************

void visitAllNodesBelowCO(
   TreeNode*         aSubjectNodeP,
   RecursiveAnchor*  aRecursiveAnchor,
   NodeVisitorCall*  aNodeVisitorCall)
{
   //-------------------------------------------------------------------------
   // Loop for each child node of the subject node

   // Recursive local variable for first in level flag, 
   bool tFirstInLevel=true;

   // Before the loop, increment the recursive level
   aRecursiveAnchor->mLevel++;

   // Start the loop at the first child node
   TreeNode* tChildNodeP = aSubjectNodeP->ptrFirstChildNode();

   // Loop through the child nodes
   while (tChildNodeP != 0)
   {
      // Update the recursive first in level flag
      aRecursiveAnchor->mFirstInLevel = tFirstInLevel;
      tFirstInLevel = false;

      // Update the recursive index
      aRecursiveAnchor->mIndex++;

      // Visit the child node before the recursive call
      (*aNodeVisitorCall)(tChildNodeP, aRecursiveAnchor);

      // Recursively call this method for the child node
      visitAllNodesBelowCO(
         tChildNodeP,
         aRecursiveAnchor,
         aNodeVisitorCall);

      // Proceed to the next child node
      tChildNodeP = tChildNodeP->ptrAfterNode();
   }

   // After the loop, decrement the recursive level
   aRecursiveAnchor->mLevel--;
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Visit all nodes below

void visitAllNodesBelowDO(
   TreeNode*         aSubjectNodeP,
   RecursiveAnchor*  aRecursiveAnchor,
   NodeVisitorCall*  aNodeVisitorCall)
{
   //-------------------------------------------------------------------------
   // Loop for each child node of the subject node

   // Recursive local variable for first in level flag, 
   bool tFirstInLevel=true;

   // Before the loop, increment the recursive level
   aRecursiveAnchor->mLevel++;

   // Start the loop at the first child node
   TreeNode* tChildNodeP = aSubjectNodeP->ptrFirstChildNode();

   // Loop through the child nodes
   while (tChildNodeP != 0)
   {
      // Update the recursive first in level flag
      aRecursiveAnchor->mFirstInLevel = tFirstInLevel;
      tFirstInLevel = false;

      // Recursively call this method for the child node
      visitAllNodesBelowDO(
         tChildNodeP,
         aRecursiveAnchor,
         aNodeVisitorCall);

      // Update the recursive index
      aRecursiveAnchor->mIndex++;

      // Visit the child node after the recursive call
      (*aNodeVisitorCall)(tChildNodeP, aRecursiveAnchor);

      // Proceed to the next child node
      tChildNodeP = tChildNodeP->ptrAfterNode();
   }

   // After the loop, decrement the recursive level
   aRecursiveAnchor->mLevel--;
}

//****************************************************************************
//****************************************************************************
//****************************************************************************

void visitNodeForPrint (TreeNode* aNode, RecursiveAnchor* aRecursiveAnchor)
{
   printf("visitNodeForPrint %3d %d %d %5d\n", 
      aRecursiveAnchor->mIndex, 
      aRecursiveAnchor->mFirstInLevel,
      aRecursiveAnchor->mLevel,
      aNode->mIdentifier);
}

void printAllTreeNodes1(TreeNode* aSubjectNodeP)
{
   NodeVisitorCall tNodeVisitorCall;
   tNodeVisitorCall.bind(visitNodeForPrint);
   RecursiveAnchor tRecursiveAnchor;

   visitAllNodesCO(
      aSubjectNodeP,
      &tRecursiveAnchor,
      &tNodeVisitorCall);
}

void printAllTreeNodes2(TreeNode* aSubjectNodeP)
{
   NodeVisitorCall tNodeVisitorCall;
   tNodeVisitorCall.bind(visitNodeForPrint);
   RecursiveAnchor tRecursiveAnchor;

   visitAllNodesDO(
      aSubjectNodeP,
      &tRecursiveAnchor,
      &tNodeVisitorCall);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Visit a node and return a pointer to the next node

TreeNode* visitNode(
   TreeNode*         aSubjectNodeP,
   RecursiveAnchor*  aRecursiveAnchor,
   NodeVisitorCall*  aNodeVisitorCall)
{
   //--------------------------------------------------------------------------
   // Visit the subject node

   (*aNodeVisitorCall)(aSubjectNodeP, aRecursiveAnchor);

   //--------------------------------------------------------------------------
   // Set the subject node ancestor with after node

   // If the subject node has no parent
   if (aSubjectNodeP->mParentNodeH == BlockHandle::nullH)
   {
      // Then it has no ancestor with after node
      aSubjectNodeP->mAncestorWithAfterH = BlockHandle::nullH;
   }
   // Else if the parent of the subject node has a node after it
   else if (aSubjectNodeP->ptrParentNode()->mAfterNodeH != BlockHandle::nullH)
   {
      // Then the subject node ancestor with after node is its parent
      aSubjectNodeP->mAncestorWithAfterH = aSubjectNodeP->mParentNodeH;
   }
   // Else the subject node has no parent with an after node
   else
   {
      // Copy the subject node's parent ancestor after node to it.
      // That will be the closest ancestor that has a node after it.
      // This is used recursively by calls to getNextNode.
      aSubjectNodeP->mAncestorWithAfterH = aSubjectNodeP->ptrParentNode()->mAncestorWithAfterH;
   }

   //--------------------------------------------------------------------------
   // Update the recursive index

   aRecursiveAnchor->mIndex++;

   //--------------------------------------------------------------------------
   // Pointer to the next node

   BlockHandle tNextNodeH = BlockHandle::nullH;

   //--------------------------------------------------------------------------
   // Determine the next node

   // If the subject node has child nodes
   if (aSubjectNodeP->mFirstChildNodeH != BlockHandle::nullH)
   {
      // The next node will be the first child node of the subject node
      tNextNodeH = aSubjectNodeP->mFirstChildNodeH;
      // The next node level will increase
      aRecursiveAnchor->mLevel++;
      // The next node will be the first in its level
      aRecursiveAnchor->mFirstInLevel = true;
   }
   // Else if the subject node has a node after it
   else if (aSubjectNodeP->mAfterNodeH != BlockHandle::nullH)
   {
      // The next node will be the node after it
      tNextNodeH = aSubjectNodeP->mAfterNodeH;
      // The next node will not be the first in its level
      aRecursiveAnchor->mFirstInLevel = false;
   }
   // Else the subject node has no child nodes and no nodes after it
   else
   {
      // If the subject node has an ancestor after node
      if (aSubjectNodeP->mAncestorWithAfterH != BlockHandle::nullH)
      {
         // Set the next node to the ancestor after node
         tNextNodeH = aSubjectNodeP->ptrAncestorWithAfter()->mAfterNodeH;
      }
   }
   // Return the next node
   return TreeNode::ptr(tNextNodeH);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
void printAllTreeNodes3(TreeNode* aSubjectNodeP)
{
   NodeVisitorCall tNodeVisitorCall;
   tNodeVisitorCall.bind(visitNodeForPrint);
   RecursiveAnchor tRecursiveAnchor;

   TreeNode* tNode = aSubjectNodeP;
   while (tNode)
   {
      tNode = visitNode(tNode, &tRecursiveAnchor, &tNodeVisitorCall);
   }
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Return a pointer to the next node, in construction order

TreeNode* getNextNode(
   TreeNode*        aSubjectNodeP)
{
   // Guard
   if (aSubjectNodeP == 0) return 0;

   //--------------------------------------------------------------------------
   // Pointer to the next node

   BlockHandle tNextNodeH = BlockHandle::nullH;

   //--------------------------------------------------------------------------
   // Set the subject node ancestor with after node

   // If the subject node has no parent
   if (aSubjectNodeP->mParentNodeH == BlockHandle::nullH)
   {
      // Then it has no ancestor with after node
      aSubjectNodeP->mAncestorWithAfterH = BlockHandle::nullH;
   }
   // Else if the parent of the subject node has a node after it
   else if (aSubjectNodeP->ptrParentNode()->mAfterNodeH != BlockHandle::nullH)
   {
      // Then the subject node ancestor with after node is its parent
      aSubjectNodeP->mAncestorWithAfterH = aSubjectNodeP->mParentNodeH;
   }
   // Else the subject node has no parent with an after node
   else
   {
      // Copy the subject node's parent ancestor after node to it.
      // That will be the closest ancestor that has a node after it.
      // This is used recursively by calls to getNextNode.
      aSubjectNodeP->mAncestorWithAfterH = aSubjectNodeP->ptrParentNode()->mAncestorWithAfterH;
   }

   //--------------------------------------------------------------------------
   // The transmit attach level starts at the subject node level

   int tTxAttachLevel = aSubjectNodeP->mTxAttachLevel;

   //--------------------------------------------------------------------------
   // If the subject node has child nodes

   if (aSubjectNodeP->mFirstChildNodeH != BlockHandle::nullH)
   {
      // The next node will be the first child node of the subject node
      tNextNodeH = aSubjectNodeP->mFirstChildNodeH;

      // Update the transmit attachment level, going down one level
      tTxAttachLevel++;
      TreeNode::ptr(tNextNodeH)->mTxAttachLevel = tTxAttachLevel;
   }
   // Else if the subject node has a node after it
   else if (aSubjectNodeP->mAfterNodeH != BlockHandle::nullH)
   {
      // The next node will be the node after it
      tNextNodeH = aSubjectNodeP->mAfterNodeH;

      // Update the transmit attachment level
      TreeNode::ptr(tNextNodeH)->mTxAttachLevel = tTxAttachLevel;
   }
   // Else the subject node has no child nodes and no nodes after it
   else
   {
      // If the subject node has an ancestor after node
      if (aSubjectNodeP->mAncestorWithAfterH != BlockHandle::nullH)
      {
         // Set the next node to the ancestor after node
         tNextNodeH = aSubjectNodeP->ptrAncestorWithAfter()->mAfterNodeH;

         // Update the transmit attachment level
         TreeNode::ptr(tNextNodeH)->mTxAttachLevel = aSubjectNodeP->ptrAncestorWithAfter()->mTxAttachLevel;
      }
   }
   // Return the next node
   return TreeNode::ptr(tNextNodeH);
}

}//namespace