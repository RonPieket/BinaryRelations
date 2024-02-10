Binary relations are everywhere
===============================

Intro
-----

There is a data structure I that has served me very well for many years in the
Insomniac Core tools group. I don’t hear other programmers refer to it, so I
want to introduce it to you. I call it a binary relation. It’s a bidirectional
associative container.

The term “binary relation” and the concept are borrowed from mathematics,
specifically set theory. But this is not a library for mathematicians. It is for
programmers needing to organize their data.

A quick refresher, before we go on. A binary relation is a collection of pairs
of elements. Each pair represents a relation between one element on the left and
one on the right. For example, on the left in the image below, the one-to-one
relation consists of four pairs, namely (A, 3), (B, 1), (C, 4), and (D, 2).

There are four kinds of binary relations. They are: one-to-one, one-to-many,
many-to-one, and many-to-many. Because one-to-many and many-to-one are
interchangeable if you swap the left and the right side, we ignore many-to-one.
You won’t need it, as will become clear later.

![](ThreeKindsOfBinaryRelation.png)

Real world examples
-------------------

The real world examples come from my experience as a game tools programmer,
specifically as a programmer of the world editor. So I will use that as an
example. I’m certain that these data structures will be useful in many other
areas.

So in the world editor then, every object in the world is represented by a
handle. They need to be organized at the global level. For example, objects can
be connected in a parent-child relationship. A parent object can have any number
of children. Each child has exactly one parent. This is a binary relation, a
one-to-many. And each child may itself be a parent and have children. So we have
a hierarchy tree.

Typically, this is kind of relationship is expressed in the object data itself.
Every object that is a child contains a handle to a parent. And each parent
contains an array of handles to its children.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class GameObject
{
    Handle m_Parent;
    std::vector<Handle> m_Children;
    ...
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This organization requires that when a child changes parent, you need to update
both the `m_Parent` of the child as well as the `m_Children` list of the parent.
This kind of connection is a one-to-many binary relation. With binary relations,
the relationship between parents and children is stored outside of the object
data structure, in its own container. All parent-to-child relationships are
stored in one container at lives alongside the game objects.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class World
{
    OneToMany<Handle, Handle> m_ParentToChildren;
    ...
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

With this data structure, you can look up the parent handle for any object, and
get a list of handles of its children.

Binary relations are everywhere
-------------------------------

Once you get the hang of storing relationships outside of the object, you will
find uses for it everywhere. For example, game objects can be member of multiple
groups. That’s a many-to-many. Given the group’s handle, you can look up all its
members. And when you have a game object, you can get a list of the groups it
belongs to.

Perhaps a more surprising example is this. Say you have an object type to
classify people, vehicles, and buildings. This is also an opportunity to use a
binary relation. In this case it’s a one-to-many. Given an object handle, you
can look up what type it is. Given an object type, you can get a list of all
objects of that type.

Here are some more use cases:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class World
{
    OneToMany<Handle, Handle>     m_ParentToChildren;
    OneToMany<ZoneId, Handle>     m_ZoneToObjects;
    OneToMany<ObjectType, Handle> m_TypeToObjects;
    OneToMany<AssetInfo, Handle>  m_AssetToNodes;
    OneToMany<AssetInfo, Handle>  m_ZoneToNodes;
    OneToMany<Handle, Handle>     m_SuperToSubNodes;
    OneToMany<AssetType, Handle>  m_AssetTypeToNodes;
    ...
    ManyToMany<Handle, Handle>    m_GroupsToObjects;
    ManyToMany<QuestId, Handle>   m_QuestsToGroups;
    ...
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The implementation
------------------

The library consists of three C++ header files: OneToOne.h, OneToMany.h, and
ManyToMany.h. There are no dependencies between these files, so you can
`#include` any combination. A fourth header file SortedVector.h is used by all
other .h files.

The library relies on the STL.
