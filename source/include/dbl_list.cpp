#ifndef DBL_LIST_CPP
#define DBL_LIST_CPP

#include"dbl_list.h"



////////////////////////////////////////////////////////////////
//                                                            //
//       SImple Double List Functions                         //
//                                                            //
////////////////////////////////////////////////////////////////

//Prepare everything
template<class T_obj>
DOUBLE_LIST<T_obj>::DOUBLE_LIST()
{
    //No elements yet
    elements = 0;
    
    //Set the pointer right
    head = NULL;
    actual = NULL;
}



//These functions adds elements to the list. Some of these holds limitations
//Returns true on success, false on fail. If success, "actual" pointer will
//point to the created node

template<class T_obj>
bool DOUBLE_LIST<T_obj>::insert(T_obj *obj)//Adds a node, default id as 0
{
    //This function will always add to the beggining of the list. Mainly
    //cuz it easy to code
    
    //No head, no ahead
    if(!head)
    {
        //Need a new head
        head = new NODE;
        
        //Simple error checking
        if(head == NULL) return(false);
        
        //Need to set the pointers right or will be sorry
        head->next = NULL;
        head->previous = NULL;
    } else {
        //There is one head already, so it must be taken care of.
        NODE *temp_node = new NODE;
        
        //Simple error checking
        if(temp_node == NULL) return(false);
        
        //Set the node's pointers right
        temp_node->next = head;
        temp_node->previous = NULL;
        
        //The head's pointer need to be corrected
        head->previous = temp_node;
        
        //Everything is done and right now, update the head one position back
        head = temp_node;
    }
    
    
    
    //Set actual
    actual = head;
    
    //Insertion has been done, put the object now
    head->obj = obj;
    head->id = 0;
    
    //One extra element
    ++elements;
    
    //Success
    return(true);
}



template<class T_obj>
bool DOUBLE_LIST<T_obj>::insert(T_obj *obj, int id)//Adds everything
{
    //Just go and insert it
    if(!insert(obj))return(false);
    
    actual->id = id;
    
    return(true);
}




template<class T_obj>
bool DOUBLE_LIST<T_obj>::operator+=(T_obj *obj)
                            //Adds a node in a cool way, id is default 0
{
    //Just go and insert it
    if(!insert(obj))return(false);
    
    return(true);
}



//A single function for UGH sorted insertion. If this will be used, be sure
//to only insert elements with this function, as it is... well, sorted.
//The other ones wont sort out anything.
template<class T_obj>
bool DOUBLE_LIST<T_obj>::sorted_insert(T_obj *obj, int id)
{
    //Pointer to the all existence of this list, the head
    NODE *temp = head;
    
    if(!head)
    {
        //If there is no head, then insertion is simple
        head = new NODE;
        temp = head;
        
        //Need to set the pointer right
        temp->next = NULL;
        temp->previous = NULL;
    } else {
        //Special case of the insertion being on second element
        if(!head->next)
        {
            if(head->id < id)
            {
                //If this element is to be on second position
                head->next = new NODE;
                temp = head->next;
                
                //The pointers.... we need them
                temp->next = NULL;
                temp->previous = NULL;
            } else {
                //Head is actually too big, need to change it
                temp = new NODE;
                
                //Set the pointers as needed
                temp->next = head;
                temp->previous = NULL;
                head->previous = temp;
                
                //Important
                head = temp;
            }
        } else {
            //All remaining positions
            
            //Insercao ordenada
            NODE *previous_node = temp;
            while(temp)
            {
                if(temp->id < id)
                {
                    //No problems. Continue
                    previous_node = temp;
                    temp = temp->next;
                } else {
                    //Jump off the loop
                    break;
                }
            }
            
            //See in what situation we are, to know were to insert
            if(!temp)
            {
                //There is no temp, so it will be to the end of the list
                temp = previous_node;
                temp->next = new NODE;
                
                //I like these weird pointers use
                temp->next->previous = temp;
                temp = temp->next;
                
                //Set it NULL to avoid some proggram deaths
                temp->next = NULL;
            } else {
                //We are not in the end of the list, but there is a few
                //situations
                if(!temp->next)
                {
                    //We add the element one position before the end of all
                    NODE *helping = new NODE;
                    
                    //Set the pointer right
                    helping->next = temp;
                    helping->previous = temp->previous;
                    helping->previous->next = helping;
                    temp->previous = helping;
                    
                    //Important to the end of the function
                    temp = helping;
                } else {
                    //Insertion somewhere at the middle
                    NODE *helping = new NODE;
                    
                    //Set the pointers
                    helping->next = temp;
                    helping->previous = temp->previous;
                    
                    if(helping->previous)
                    {
                        helping->previous->next = helping;
                    } else {
                        head = helping;
                    }
                    
                    helping->next->previous = helping;
                    
                    //Importat
                    temp = helping;
                }
            }
        }
    }
    
    //Set the values as we wanted
    temp->obj = obj;
    temp->id = id;
    
    //One more object to the list
    ++elements;
    
    return(true);
}



//This remove function receives a pointer to the node to be removed.
//The object it pointed to is not deleted, giving chance for flexible
//use and memory leaks.
template<class T_obj>
bool DOUBLE_LIST<T_obj>::private_remove(NODE *node)
{
    //Small error checking
    if(!node)return(false);
    
    //Put the "actual" pointer on right place
    if(actual == node) actual = actual->previous;
    
    //If the node is actually the head
    if(node == head)
    {
        //Then head goes ahead and node dies
        head = head->next;
        
        if(head) head->previous = NULL;
        
        delete node;
    } else {
        if(node->next)
        {
            //The node has friends on both sides. Correct the pointers
            //and then this node dies
            node->previous->next = node->next;
            node->next->previous = node->previous; //Double tricky XD
            
            delete node;
        } else {
            //If the node is the one at the final of the list
            
            //Previous node points to null, and this node dies
            node->previous->next = NULL; //Single tricky =(
            
            delete node;
        }
    }
    
    //One less element
    --elements;
    
    //Success achieved
    return(true);
}



//The delete function works exactly the same as remove function, but
//deletes the object. Avoids memory leaks, but opens space for illegal
//access =/
template<class T_obj>
bool DOUBLE_LIST<T_obj>::private_delete(NODE *node)
{
    //Small error checking
    if(!node)return(false);
    
    //Put the "actual" pointer on right place
    if(actual == node) actual = actual->previous;
    
    //If the node is actually the head
    if(node == head)
    {
        //Then head goes ahead and node dies
        head = head->next;
        
        if(head) head->previous = NULL;
        
        delete node->obj;
        delete node;
    } else {
        if(node->next)
        {
            //The node has friends on both sides. Correct the pointers
            //and then this node dies
            node->previous->next = node->next;
            node->next->previous = node->previous; //Double tricky XD
            
            delete node->obj;
            delete node;
        } else {
            //If the node is the one at the final of the list
            
            //Previous node points to null, and this node dies
            node->previous->next = NULL; //Single tricky =(
            
            delete node->obj;
            delete node;
        }
    }
    
    //One less element
    --elements;
    
    //Success achieved
    return(true);
}



//These functions remove the element and destroy the node. The removed
//object is also DELETED from memory, so, beware.

template<class T_obj>
bool DOUBLE_LIST<T_obj>::delete_obj()//Remove what "actual" pointer is pointing right now.
                                     //"actual" will then point to previous.
{
    return(private_delete(actual));
}



template<class T_obj>
bool DOUBLE_LIST<T_obj>::delete_obj(T_obj *obj)//Removes giving its pointer
{
    //Runs a search to get a pointer to the node we want to remove.
    //Remove it if possible, and return success or fail
    private_search(obj);
    return(private_delete(search_results));
}



template<class T_obj>
bool DOUBLE_LIST<T_obj>::delete_obj(int id)//Remove by means of ID. The first one found, only.
{
    //Runs a search to get a pointer to the node we want to remove.
    //Remove it if possible, and return success or fail
    private_search(id);
    return(private_delete(search_results));
}



//These functions only destroy the node, but the object it pointed to
//will stay on memory.
template<class T_obj>
bool DOUBLE_LIST<T_obj>::remove_obj()//Uses the "actual" pointer.
{
    //Uses "actual" for removing
    return(private_remove(actual));
}



template<class T_obj>
bool DOUBLE_LIST<T_obj>::remove_obj(int id)//Remove by knowing its id. First found, actually.
{
    //Runs a search to get a pointer to the node we want to remove.
    //Remove it if possible, and return success or fail
    private_search(id);
    return(private_remove(search_results));
}



template<class T_obj>
bool DOUBLE_LIST<T_obj>::remove_obj(T_obj *obj)//Removes giving the object address
{
    //Runs a search to get a pointer to the node we want to remove.
    //Remove it if possible, and return success or fail
    private_search(obj);
    return(private_remove(search_results));
}



//These search functions does not set actual to anything. They search
//for an element and return its node address. These ones are used by this
//class only.
template<class T_obj>
void DOUBLE_LIST<T_obj>::private_search(int id)//by id
{
    //Starts as head
    NODE *temp = head;
    
    //Run a searching loop
    while(temp)
    {
        //If found, out of the loop and function
        if(temp->id == id)
        {
            search_results = temp;
            return;
        }
        
        //If not, next element
        temp = temp->next;
    }
    
    //Nothing was found
    search_results = NULL;
}



template<class T_obj>
void DOUBLE_LIST<T_obj>::private_search(T_obj *obj)//By objects address
{
    //Starts as head
    NODE *temp = head;
    
    //Run a searching loop
    while(temp)
    {
        //If found, out of the loop and function
        if(temp->obj == obj)
        {
            search_results = temp;
            return;
        }
        
        //If not, next element
        temp = temp->next;
    }
    
    //Nothing was found
    search_results = NULL;
}



//Public search functions. Kinda useful. They return true on finding and "actual"
//pointer will point to the node. If not found, "actual" points to NULL.

template<class T_obj>
bool DOUBLE_LIST<T_obj>::search(T_obj *obj)//Weird search, using object address
{
    //Starts as head
    actual = head;
    
    //Run a searching loop
    while(actual)
    {
        //If found, out of the loop and function
        if(actual->obj == obj) return(true);
        
        //If not, next element
        actual = actual->next;
    }
    
    //Nothing was found
    return(false);
}



template<class T_obj>
T_obj* DOUBLE_LIST<T_obj>::search(int id)//First node with this id will appear
{
    //Starts as head
    actual = head;
    
    //Run a searching loop
    while(actual)
    {
        //If found, out of the loop and function
        if(actual->id == id) return(actual->obj);
        
        //If not, next element
        actual = actual->next;
    }
    
    //Nothing was found
    return(NULL);
}



//Pointers used to navigate through the list. They return the address
//of the object inside the node the function moved into, and actual
//will also point to it. NULL on no object, both actual and return value

template<class T_obj>
T_obj* DOUBLE_LIST<T_obj>::next()
{
    if(actual)
    {
        actual = actual->next;
        
        if(!actual) return(NULL);
        return(actual->obj);
    } else {
        return(NULL);
    }
}



template<class T_obj>
T_obj* DOUBLE_LIST<T_obj>::previous()
{
    if(actual)
    {
        actual = actual->previous;
        
        if(!actual) return(NULL);
        return(actual->obj);
    } else {
        return(NULL);
    }
}



template<class T_obj>
T_obj* DOUBLE_LIST<T_obj>::DOUBLE_LIST::operator++(void)//Next
{
    if(actual)
    {
        actual = actual->next;
        
        if(!actual) return(NULL);
        return(actual->obj);
    } else {
        return(NULL);
    }
}



template<class T_obj>
T_obj* DOUBLE_LIST<T_obj>::DOUBLE_LIST::operator--(void)//Previous
{
    if(actual)
    {
        actual = actual->previous;
        
        if(!actual) return(NULL);
        return(actual->obj);
    } else {
        return(NULL);
    }
}



//Get what "actual" is pointing now
template<class T_obj>
T_obj* DOUBLE_LIST<T_obj>::get_obj()
{
    if(actual)
    {
        //Give it away
        return(actual->obj);
    } else {
        //Sorry
        return(NULL);
    }
}



//Return the number of elements
template<class T_obj>
int DOUBLE_LIST<T_obj>::get_elements()
{
    return(elements);
}



//Set "actual" to the head of the list, or NULL, if there is not one
template<class T_obj>
void DOUBLE_LIST<T_obj>::reset_actual(void)
{
    //Yeah, this simple.
    actual = head;
}



//Returns true if "actual" is not NULL
template<class T_obj>
bool DOUBLE_LIST<T_obj>::actual_valid()
{
    return(actual != NULL ? true : false);
}


    
//Clear the list, and remove all objects from memory. This means, they
//dont need to be manually deleted somewhere else
template<class T_obj>
void DOUBLE_LIST<T_obj>::clear()
{
    //Set a helping variable
    NODE *temp_node = head;
    
    //If there is a head, there is a kill
    while(head)
    {
        //Make the head point to the next head
        head = head->next;
        
        //Remove this node object
        delete temp_node->obj;
        
        //Suicide the node
        delete temp_node;
        
        //Hunts the new head
        temp_node = head;
    }
    
    //No more elements
    elements = 0;
    
    //Actual is NULL
    actual = NULL;
}



//Clean the lists. This does not delete the objects, so careful with
//memory leaks
template<class T_obj>
void DOUBLE_LIST<T_obj>::clean()
{
    //Set a helping variable
    NODE *temp_node = head;
    
    //If there is a head, there is a kill
    while(head)
    {
        //Make the head point to the next head
        head = head->next;
        
        //Suicide the node
        delete temp_node;
        
        //Hunts the new head
        temp_node = head;
    }
    
    //No more elements
    elements = 0;
    
    //Actual is NULL
    actual = NULL;
}



//Destructor
template<class T_obj>
DOUBLE_LIST<T_obj>::~DOUBLE_LIST()
{
    //Unload it all
    clear();
}

#endif
