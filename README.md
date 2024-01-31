Binary Relations
----------------

There is a data structure I that has served me very well for the last seven
years in the Insomniac Core tools group. I don’t hear other programmers refer to
it, so I want to introduce it to you. I call it a binary relation. It’s an
associative container with a reverse look-up.

The Missing Container
---------------------

The four associative container types we know from STL are: set, map, multiset,
multimap.

Real world examples
-------------------

Tools programmer, thus using world editor examples. Scene nodes, model
instances, zones. But applicable to many other systems that require organization
- i.e. most of them. AI, rendering, physics, animation. Handy for
collection-based programming.

Reverse look-ups are common
---------------------------

A versatile hammer

Reverse look-ups where you didn’t expect them
---------------------------------------------

Attributes, groups, classification.

Three kinds of binary relations
-------------------------------

The three kinds of binary relations are: one-to-one, one-to-many, and
many-to-many. The many-to-one binary relation is just the inverse of a
one-to-many, so it is omitted.

Use cases
---------

-   ParentToChildren

-   TypeToNodes

-   ZoneToNodes

-   AssetToNodes

-   SelectionToNodes

 

Multiple, simultaneous hierarchies. Trees. Flags. Attributes.

 

Binary relations for tree hierarchies
-------------------------------------

Binary relations for grouping by attribute
------------------------------------------

Filtering by attribute combination
----------------------------------

Implementation with sorted vectors, a compromise
------------------------------------------------

Sorted vectors and set operations
---------------------------------

 
