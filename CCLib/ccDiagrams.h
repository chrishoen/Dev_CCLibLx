#pragma once

/*==============================================================================

Construction Order, parent nodes are created before child nodes

   +---------+            +---------+
   |         |----------->|         |
   |    0    |------------|    1    |
   |         |            |         |
   +---------+            +---------+
                               |  |
                               |  |
                               |  |
                               |  v
                          +---------+            +---------+            +---------+            +---------+
                          |         |----------->|         |----------->|         |----------->|         |
                          |    2    |------------|    3    |------------|    4    |------------|    5    |
                          |         |            |         |            |         |            |         |
                          +---------+            +---------+            +---------+            +---------+
                               |                     |                                              |
                               |                     |  +-------------------------------------------+ 
                               |                     |  |
                               |                     |  v 
                               |                +---------+            +---------+
                               |                |         |----------->|         |   
                               |                |    6    |------------|    7    |
                               |                |         |            |         | 
                               |                +---------+            +---------+
                               |                     |                      |  |
                               |                     |                      |  |
                               |                     |                      |  |
                               |                     |                      |  v
                               |                     |                 +---------+
                               |                     |                 |         |   
                               |                     |                 |    8    |
                               |                     |                 |         | 
                               |                     |                 +---------+
                               |                     |                      |
                               |                     |  +-------------------+
                               |                     |  |
                               |                     |  v
                               |                +---------+
                               |                |         |   
                               |                |    9    |
                               |                |         | 
                               |                +---------+
                               |                     | 
                               |  +------------------+
                               |  |
                               |  v
                          +---------+
                          |         |   
                          |   10    |
                          |         | 
                          +---------+
                               |
                               v
                              Exit

Destruction Order, child nodes are destroyed before parent nodes

   +---------+            +---------+
   |         |            |         |
   |   10    |------------|    0    |
   |         |            |         |
   +---------+            +---------+
                               |   
                               |   
                               |    
                               |   
                          +---------+            +---------+            +---------+            +---------+
                          |         |            |         |            |         |            |         |
                          |    4    |------------|    3    |------------|    2    |------------|    1    |
                          |         |            |         |            |         |            |         |
                          +---------+            +---------+            +---------+            +---------+
                               |                     |                                               
                               |                     |                                                
                               |                     |   
                               |                     |    
                               |                +---------+            +---------+
                               |                |         |            |         |   
                               |                |    6    |------------|    5    |
                               |                |         |            |         | 
                               |                +---------+            +---------+
                               |                     |                      |   
                               |                     |                      |   
                               |                     |                      |   
                               |                     |                      |   
                               |                     |                 +---------+
                               |                     |                 |         |   
                               |                     |                 |    7    |
                               |                     |                 |         | 
                               |                     |                 +---------+
                               |                     |                       
                               |                     |                       
                               |                     |   
                               |                     |   
                               |                +---------+
                               |                |         |   
                               |                |    8    |
                               |                |         | 
                               |                +---------+
                               |                       
                               |                      
                               |   
                               |   
                          +---------+
                          |         |   
                          |    9    |
                          |         | 
                          +---------+

==============================================================================*/
