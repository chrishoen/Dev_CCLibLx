#ifndef _CCDIAGRAMS_H_
#define _CCDIAGRAMS_H_
/*==============================================================================

FREE LIST

   +---+     +---+     +---+     +---+     +---+     +---+        init
  H| 0 |    H| 1 |   H | 2 |   H | 3 |    H| 0 |    H| 1 |        pop  0
   +---+     +---+     +---+     +---+     +---+     +---+        pop  1 
     v         v         v         v         v         v          pop  2
   +---+     +---+     +---+     +---+     +---+     +---+        push 0
   | 1 |     | 2 |     | 3 |     | 4 |     | 3 |     | 0 |        push 1
   +---+     +---+     +---+     +---+     +---+     +---+
     v         v         v                   v         v
   +---+     +---+     +---+               +---+     +---+
   | 2 |     | 3 |     | 4 |               | 4 |     | 3 |
   +---+     +---+     +---+               +---+     +---+
     v         v                                       v
   +---+     +---+                                   +---+
   | 3 |     | 4 |                                   | 4 |
   +---+     +---+                                   +---+
     v                                                  
   +---+                                                  
   | 4 |                                                  
   +---+                                                  

QUEUE
             +---+     +---+     +---+     +---+     +---+
          HT | 0 |    H| 0 |    H| 0 |    H| 1 |   HT| 2 |  HEAD IS DUMMY NODE
             +---+     +---+     +---+     +---+     +---+
                         v         v         v
                       +---+     +---+     +---+          
                      T| 1 |     | 1 |   T | 2 |          
                       +---+     +---+     +---+          
                                   v                    
                                 +---+                    
                               T | 2 |                    
                                 +---+                    

PUSH A: A.N = H                     ENQ A:   POP A
        H = A                                A.V = V 
                                             T.N = A 
POP B:  H = B.N                              T = A

                                    DEQ H.N TEMP = H
                                            H = TEMP.N
                                            V = TEMP.N.V
                                            PUSH TEMP

==============================================================================*/


/*==============================================================================
Non-blocking queue [Michael and Scott algorithm]

structure pointer_t {ptr:   pointer to node_t, count: unsigned integer}
structure node_t    {value: data_type,         next: pointer_t}
structure queue_t   {Head:  pointer t,         Tail: pointer_t}


initialize(Q: pointer to queue_t)
      node = new node()                                                # Allocate a free node
      node–>next.ptr = NULL                                            # Make it the only node in the linked list
      Q–>Head = Q–>Tail = node                                         # Both Head and Tail point to it



enqueue(Q: pointer to queue_t, value: data type)
E1:   node = new node()                                                # Allocate a new node from the free list  
E2:   node–>value = value                                              # Copy enqueued value into node
E3:   node–>next.ptr = NULL                                            # Set next pointer of node to NULL
E4:   loop                                                             # Keep trying until Enqueue is done
E5:       tail = Q–>Tail                                               # Read Tail.ptr and Tail.count together
E6:       next = tail.ptr–>next                                        # Read next ptr and count fields together
E7:       if tail == Q–>Tail                                           # Are tail and next consistent?
E8:           if next.ptr == NULL                                      # Was Tail pointing to the last node?
E9:               if CAS(&tail.ptr–>next, next, <node, next.count+1>)  # Try to link node at the end of the linked list
E10:                  break                                            # Enqueue is done. Exit loop
E11:              endif
E12:          else                                                     # Tail was not pointing to the last node
E13:              CAS(&Q–>Tail, tail, <next.ptr, tail.count+1>)        # Try to swing Tail to the next node
E14:          endif
E15:      endif
E16:    endloop
E17:    CAS(&Q–>Tail, tail, <node, tail.count+1>)                      # Enqueue is done. Try to swing Tail to the






dequeue(Q: pointer to queue t, pvalue: pointer to data type): boolean
D1:   loop                                                             # Keep trying until Dequeue is done
D2:       head = Q–>Head                                               # Read Head
D3:       tail = Q–>Tail                                               # Read Tail
D4:       next = head–>next                                            # Read Head.ptr–>next
D5:       if head == Q–>Head                                           # Are head, tail, and next consistent?
D6:           if head.ptr == tail.ptr                                            # Is queue empty or Tail falling behind?
D7:               if next.ptr == NULL                                  # Is queue empty?
D8:                   return FALSE                                     # Queue is empty, couldn’t dequeue
D9:               endif
D10:              CAS(&Q–>Tail, tail, <next.ptr, tail.count+1>)        # Tail is falling behind. Try to advance it
D11:          else                                                     # No need to deal with Tail
D12:              *pvalue = next.ptr–>value                            # Read value before CAS, otherwise another dequeue might free the next node
D13:              if CAS(&Q–>Head, head, <next.ptr, head.count+1>)     # Try to swing Head to the next node
D14:                  break # Dequeue is done. Exit loop
D15:              endif
D16:          endif
D17:      endif
D18:  endloop
D19: free(head.ptr)                                                    # It is safe now to free the old dummy node
D20: return TRUE                                                       # Queue was not empty, dequeue succeeded









Non-blocking stack [Treiber’s algorithm]

proc push(new)
do
  old = top
  new.next = old
while not CAS(top, old, new)
end

proc pop
do
  old = top
  return null if old == null
  new = old.next
while not CAS(top, old, new)
return old
end



==============================================================================*/
#endif

#if 0

   FOR REFERENCE, HERE IS THE ABOVE CODE WITHOUT THE ATOMICS

   bool tryWrite(int aValue)
   {
      // Try to allocate a node from the free list.
      // Exit if it is empty.
      int tNode;
      if (!listPop(&tNode)) return false;

      // Initialize the node with the value.
      mNode[tNode].mValue = aValue;
      mNode[tNode].mQueueNext = cInvalid;

      // Attach the node to the queue tail.
      int tQueueTail;
      mWriteRetry--;
      while (true)
      {
         mWriteRetry++;
         tQueueTail = mQueueTail;

         // Update the tail next index to point to the node. It should be
         // invalid, if not then there was a collision.
         int tInvalid = cInvalid;
         if (mNode[tQueueTail].mQueueNext.compare_exchange_weak(tInvalid, tNode)) break;
         // If the above line fails then the tail next index was not updated, 
         // so advance the tail.
         mQueueTail.compare_exchange_weak(tQueueTail, mNode[tQueueTail].mQueueNext);
      }
      // Update the tail index so that the node is the new tail.
      mQueueTail.compare_exchange_strong(tQueueTail, tNode);

      // Done
      return true;
   }

   bool tryWrite (int aValue)
   {
      // Try to allocate a node from the free list.
      // Exit if it is empty.
      int tNode;
      if (!listPop(&tNode)) return false;

      // Initialize the node with the value.
      mNode[tNode].mValue = aValue;
      mNode[tNode].mQueueNext = cInvalid;

      // Attach the node to the queue tail node and update the tail index.
      mNode[mQueueTail].mQueueNext = tNode;
      mQueueTail = tNode;

      // Done
      return true;
   }

   bool tryRead(int* aReadValue)
   {
      // Store the head node in a temp.
      int tQueueHead = mQueueHead;
      mReadRetry--;
      while (true)
      {
         mReadRetry++;
         // Exit if the queue is empty.
         if (mNode[tQueueHead].mQueueNext == cInvalid) return false;

         if (mQueueHead.compare_exchange_weak(tQueueHead, mNode[tQueueHead].mQueueNext)) break;
      }
      // Extract the read value from the head block.
      int tReadNode = mNode[tQueueHead].mQueueNext;
      *aReadValue = mNode[tReadNode].mValue;

      // Push the previous head node back onto the free list.
      listPush(tQueueHead);

      // Done.
      return true;
   }

   bool tryRead (int* aReadValue) 
   {
      // Store the read node index in a temp.
      int tReadNode = mNode[mQueueHead].mQueueNext;

      // Exit if the queue is empty.
      if (tReadNode == cInvalid) return false;

      // Extract the value from the read node.
      *aReadValue = mNode[tReadNode].mValue;

      // Push the previous head node back onto the free list.
      listPush(mQueueHead);

      // Update the head node to be the one that was just read from.
      mQueueHead = tReadNode;

      // Done.
      return true;
   }


   bool listPush (int aNode)
   {
      // Exit if the list is full.
      if (mListSize >= mAllocate) return false;

      // Attach the node that is after the head to the node.
      mNode[aNode].mListNext = mNode[mListHead].mListNext.load();

      // Attach the node to the head.
      mNode[mListHead].mListNext = aNode;

      // Done.
      mListSize++;
      return true;
   }

   bool listPop (int* aNode)
   {
      // Store the node that is after the head in a temp.
      // This is the node that will be detached.
      int tNode = mNode[mListHead].mListNext;

      // Exit if the queue is empty.
      if (tNode == cInvalid) return false;

      // Detach the node.
      mNode[mListHead].mListNext = mNode[tNode].mListNext.load();

      // Reset the detached node.
      mNode[tNode].mValue = 0;
      mNode[tNode].mQueueNext = cInvalid;
      mNode[tNode].mListNext  = cInvalid;

      // Return result.
      *aNode = tNode;

      // Done.
      mListSize--;
      return true;
   }
}

#endif
