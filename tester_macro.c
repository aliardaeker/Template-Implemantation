#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dlfcn.h>
#include <stdint.h>
#include <random>
#include <unistd.h>
#include <algorithm>
#include <typeinfo>

#define Deque_DEFINE(type)													\
																	\
	struct Deque_##type##_Iterator 	      									\
	{																\
      	type * iter;													\
      																\
      	void (* inc) (Deque_##type##_Iterator *);									\
      	void (* dec) (Deque_##type##_Iterator *);									\
      																\
      	type & (* deref) (Deque_##type##_Iterator *);								\
	};																\
																	\
	type & deref_def(Deque_##type##_Iterator * deq_iter_ptr)							\
	{																\
		return *(deq_iter_ptr -> iter);										\
	}																\
																	\
	void inc_def(Deque_##type##_Iterator * deq_iter_ptr)								\
	{				                                                                        \
            /*  Iterators might need wraping around */								\
		deq_iter_ptr -> iter++;												\
	}																\
																	\
	void dec_def(Deque_##type##_Iterator * deq_iter_ptr)								\
	{					                                                                  \
            /*  Iterators might need wraping around */								\
		deq_iter_ptr -> iter--;												\
	}																\
																	\
	bool																\
	Deque_##type##_Iterator_equal(Deque_##type##_Iterator it, Deque_##type##_Iterator end)		\
	{																\
		/* Returns True if the iterators point to the same MyClass object */				\
	      if (it.iter == end.iter) return true;									\
	      else return false; 												\
	}																\
																	\
	struct Deque_##type							\
	{										\
		type * data;							\
		int capacity;							\
    		size_t length;	                                    \
            					                   	\
    		int front_index;						      \
    		int back_index;						      \
    		char type_name[sizeof("Deque_"#type)];	  		\
    											\
    		size_t (* size) (Deque_##type * deq_ptr);  		\
    		bool (* empty) (Deque_##type * deq_ptr);			\
    											\
    		void (* push_back) (Deque_##type *, type);		\
    		void (* push_front) (Deque_##type *, type);		\
    											\
      	type & (* back) (Deque_##type *);				\
      	type & (* front) (Deque_##type *);				\
      										\
      	void (* pop_back) (Deque_##type *);				\
      	void (* pop_front) (Deque_##type *);			\
      										\
      	Deque_##type##_Iterator begin_iter;				\
    											\
    		Deque_##type##_Iterator (* begin) (Deque_##type *);	\
      	Deque_##type##_Iterator (* end) (Deque_##type *);	\
      										\
      	type & (* at) (Deque_##type * deq_ptr, size_t index);	\
            void (* clear) (Deque_##type * deq_ptr);              \
            void (* dtor) (Deque_##type * deq_ptr);               \
                                                                                                \
            void (* sort) (Deque_##type *, Deque_##type##_Iterator, Deque_##type##_Iterator);   \
            bool (* comparator) (const type &, const type &);                                   \
	};										\
											\
	size_t 									\
	size_def(Deque_##type * deq_ptr)					\
	{										\
		return deq_ptr -> length;					\
	}										\
											\
	bool										\
	empty_def(Deque_##type * deq_ptr)					\
	{										\
		if (deq_ptr -> length) return false;			\
		else return true;							\
	}										\
											\
	void push_back_def(Deque_##type * deq_ptr, type obj)		\
	{										\
      	deq_ptr -> data[deq_ptr -> back_index] = obj;         \
            deq_ptr -> back_index = (deq_ptr -> back_index + 1) % deq_ptr -> capacity;	      \
      	deq_ptr -> length++;						                              \
	}										                              \
											                              \
	void push_front_def(Deque_##type * deq_ptr, type obj)		                              \
	{										                              \
	      deq_ptr -> data[deq_ptr -> front_index] = obj;		                              \
     		deq_ptr -> front_index--;                                                           \
                                                                                                \
            if (deq_ptr -> front_index < 0) deq_ptr -> front_index += deq_ptr -> capacity;	\
     		deq_ptr -> length++;						\
	}										\
											\
	type & back_def(Deque_##type * deq_ptr)				\
	{										\
    		return deq_ptr -> data[deq_ptr -> back_index - 1];	\
	}										\
											\
	type & front_def(Deque_##type * deq_ptr)				\
	{										\
	    return deq_ptr -> data[deq_ptr -> front_index + 1];	\
	}										\
											\
	void pop_back_def(Deque_##type * deq_ptr)				\
	{										\
      	deq_ptr -> back_index--;                              \
                                                                  \
            if (deq_ptr -> back_index < 0) deq_ptr -> back_index += deq_ptr -> capacity;		\
      	deq_ptr -> length--;						                              \
	}										                              \
											                              \
	void pop_front_def(Deque_##type * deq_ptr)			                              \
	{										                              \
	      deq_ptr -> front_index = (deq_ptr -> front_index + 1) % deq_ptr -> capacity;		\
	      deq_ptr -> length--;						                              \
	}										                              \
																\
	Deque_##type##_Iterator begin_def(Deque_##type * deq_ptr)						\
	{															\
		(deq_ptr -> begin_iter).iter = &(deq_ptr -> data[deq_ptr -> front_index + 1]);	\
      	return deq_ptr -> begin_iter;										\
	}															\
																\
	Deque_##type##_Iterator end_def(Deque_##type * deq_ptr)						\
	{															\
		(deq_ptr -> begin_iter).iter = &(deq_ptr -> data[deq_ptr -> back_index]);		\
	      return deq_ptr -> begin_iter;										\
	}															\
																\
	type & at_def(Deque_##type * deq_ptr, size_t index)							\
	{															\
		int target = deq_ptr -> front_index + 1 + index;                                    \
            if (target == deq_ptr -> capacity) target -= deq_ptr -> capacity;                   \
                                                                                                \
            return deq_ptr -> data[target];				                              \
	}															\
                                                                                                \
      void clear_def(Deque_##type * deq_ptr)                                                    \
      {                                                                                         \
            /* Should I remove the elements here ? if so what should be the behavior */         \
            deq_ptr -> length = 0;                                                              \
            deq_ptr -> back_index = deq_ptr -> capacity / 2;                                    \
            deq_ptr -> front_index = deq_ptr -> back_index - 1;                                 \
      }                                                                                         \
                                                                                                \
      void dtor_def(Deque_##type * deq_ptr)                                                     \
      {                                                                                         \
            free(deq_ptr -> data);                                                              \
      }                                                                                         \
																\
      void sort_swap(type * i, type * j)                                                        \
      {                                                                                         \
            type tmp = *i;                                                                      \
            *i = *j;                                                                            \
            *j = tmp;                                                                           \
      }                                                                                         \
                                                                                                \
      Deque_##type##_Iterator sort_partition(Deque_##type * deq, Deque_##type##_Iterator low, Deque_##type##_Iterator high)   \
      {                                                                                                                       \
            type * pivot = high.iter;                                                                                         \
                                                                                                                              \
            Deque_##type##_Iterator i;                                                                                        \
            i.iter = low.iter - 1;                                                                                            \
                                                                                                                              \
            for (type * j = low.iter; j <= high.iter - 1; j++)                                                                \
            {                                                                                                                 \
                  /* returns True if first obj is smaller */                                                                  \
                  if (deq -> comparator(*j, *pivot))                                                                          \
                  {                                                                                                           \
                        i.iter++;                                                                                             \
                        sort_swap(i.iter, j);                                                                                 \
                  }                                                                                                           \
            }                                                                                                                 \
                                                                                                                              \
            i.iter++;                                                                                                         \
            sort_swap(i.iter, high.iter);                                                                                     \
                                                                                                                              \
            return i;                                                                                                         \
      }                                                                                                                       \
                                                                                                                              \
      void sort_orig(Deque_##type * deq_ptr, Deque_##type##_Iterator beg, Deque_##type##_Iterator end)                        \
      {                                                                                                                       \
            if (beg.iter < end.iter)                                                                                          \
            {                                                                                                                 \
                  Deque_##type##_Iterator pi = sort_partition(deq_ptr, beg, end);                                             \
                                                                                                                              \
                  pi.iter--;                                                                                                  \
                  sort_orig(deq_ptr, beg, pi);                                                                                \
                                                                                                                              \
                  pi.iter += 2;                                                                                               \
                  sort_orig(deq_ptr, pi, end);                                                                                \
            }                                                                                                                 \
      }                                                                                                                       \
                                                                                                                              \
      void sort_def (Deque_##type * deq_ptr, Deque_##type##_Iterator beg, Deque_##type##_Iterator end)                        \
      {                                                                                                                       \
            end.iter--; /* Because end iterator is end + 1 (back_index) */                                                    \
                                                                                                                              \
            if (beg.iter < end.iter)                                                                                          \
            {                                                                                                                 \
               Deque_##type##_Iterator pi = sort_partition(deq_ptr, beg, end);                                                \
                                                                                                                              \
                pi.iter--;                                                                                                    \
                sort_orig(deq_ptr, beg, pi);                                                                                  \
                                                                                                                              \
                pi.iter += 2;                                                                                                 \
                sort_orig(deq_ptr, pi, end);                                                                                  \
            }                                                                                                                 \
      }                                                                                                                       \
                                                                                                                              \
	void 															      \
	Deque_##type##_ctor(Deque_##type * deq_ptr, bool (* cmp) (const type &, const type &))	      \
	{																\
		/* That will call malloc in test.cpp */									\
		deq_ptr -> capacity = 100000;       									\
		deq_ptr -> data = (type *) malloc(deq_ptr -> capacity * sizeof(type));				\
																	\
		deq_ptr -> length = 0;												\
		deq_ptr -> back_index = deq_ptr -> capacity / 2;							\
		deq_ptr -> front_index = deq_ptr -> back_index - 1;							\
																	\
		strcpy(deq_ptr -> type_name, "Deque_"#type);								\
		deq_ptr -> size = size_def;											\
		deq_ptr -> empty = empty_def;											\
																	\
		deq_ptr -> push_back = push_back_def;									\
		deq_ptr -> push_front = push_front_def;									\
																	\
      	deq_ptr -> back = back_def;											\
      	deq_ptr -> front = front_def;											\
      																\
      	deq_ptr -> pop_back = pop_back_def;										\
      	deq_ptr -> pop_front = pop_front_def;									\
      																\
      	deq_ptr -> begin = begin_def;											\
      	deq_ptr -> end = end_def;											\
      																\
      	(deq_ptr -> begin_iter).inc = inc_def;									\
      	(deq_ptr -> begin_iter).dec = dec_def;									\
      	(deq_ptr -> begin_iter).deref = deref_def;								\
      																\
      	deq_ptr -> at = at_def;		    		                                                \
            deq_ptr -> clear = clear_def;                                                             \
            deq_ptr -> dtor = dtor_def;					                      			\
                                                                                                      \
            deq_ptr -> sort = sort_def;                                                               \
            deq_ptr -> comparator = cmp;                                                              \
	}							                                                      \
                                                                                                      \
      bool                                                                                            \
      Deque_##type##_equal(Deque_##type deq1, Deque_##type deq2)                                      \
      {                                                                                               \
            /* Returns true if deqs are same length and all elements are equal */                     \
            if (deq1.length != deq2.length) return false;                                             \
                                                                                                      \
            for (size_t i = 0; i < deq1.length; i++)                                                  \
            {                                                                                         \
                  type a = deq1.data[i + deq1.front_index + 1];                                       \
                  type b = deq2.data[i + deq2.front_index + 1];                                       \
                                                                                                      \
                  if (deq1.comparator(&a, &b)) return false;                                          \
                  if (deq1.comparator(&a, &b)) return false;                                          \
            }                                                                                         \
                                                                                                      \
            return true;                                                                              \
      }                                                                                               

struct MyClass
{
    	int id;
    	char name[10];
};

void
MyClass_print(const MyClass * o) 
{
    printf("%d\n", o -> id);
    printf("%s\n", o -> name);
}

bool
MyClass_less_by_id(const MyClass & o1, const MyClass & o2) 
{
      return o1.id < o2.id;
}

bool
MyClass_less_by_name(const MyClass & o1, const MyClass & o2) 
{
    return strcmp(o1.name, o2.name) < 0;
}

Deque_DEFINE(MyClass)
Deque_DEFINE(int)

bool
int_less(const int &o1, const int &o2) 
{
    return o1 < o2;
}

int main() 
{
      Deque_int deq;
      Deque_int_ctor(&deq, int_less);

      assert(deq.size(&deq) == 0);
      assert(deq.empty(&deq));

      // Should print "---- Deque_int, 10".
      //printf("---- %s, %d\n", deq.type_name, int(sizeof(deq.type_name)));
      // std::cout << "---- " << deq.type_name << ", " << sizeof(deq.type_name) << std::endl;
      assert(sizeof deq.type_name == 10);

      deq.push_back(&deq, 1);
      deq.push_back(&deq, 2);
      deq.push_back(&deq, 3);
      deq.push_front(&deq, 0);
      deq.push_front(&deq, -1);

      //printf("%d\n", deq.front(&deq));
      //printf("%d\n", deq.back(&deq));
      assert(deq.front(&deq) == -1);
      assert(deq.back(&deq) == 3);

      deq.pop_front(&deq);
      deq.pop_back(&deq);
      assert(deq.front(&deq) == 0);
      assert(deq.back(&deq) == 2);

      assert(deq.size(&deq) == 3);

      for (Deque_int_Iterator it = deq.begin(&deq); !Deque_int_Iterator_equal(it, deq.end(&deq)); it.inc(&it)) 
      {
            //printf("%d\n", it.deref(&it));
      }

      // Test decrement.
      {
            auto it = deq.end(&deq);
            it.dec(&it);
            assert(it.deref(&it) == 2);
      }

      // printf("Using at.\n");
      for (size_t i = 0; i < 3; i++) 
      {
            //printf("%d: %d\n", int(i), deq.at(&deq, i));
      }

      deq.clear(&deq);
      deq.dtor(&deq);

      {
            Deque_int deq1, deq2;
            Deque_int_ctor(&deq1, int_less);
            Deque_int_ctor(&deq2, int_less);

            deq1.push_back(&deq1, 1);
            deq1.push_back(&deq1, 2);
            deq1.push_back(&deq1, 3);
            deq2.push_back(&deq2, 1);
            deq2.push_back(&deq2, 2);
            deq2.push_back(&deq2, 3);

            assert(Deque_int_equal(deq1, deq2));

            deq1.pop_back(&deq1);
            assert(!Deque_int_equal(deq1, deq2));
            deq1.push_back(&deq1, 4);
            assert(!Deque_int_equal(deq1, deq2));

            deq1.dtor(&deq1);
            deq2.dtor(&deq2);
      }

        // Test equality.  Two deques compare equal if they are of the same
        // length and all the elements compare equal.  It is undefined behavior
        // if the two deques were constructed with different comparison
        // functions.
        {
            Deque_MyClass deq1, deq2;
            Deque_MyClass_ctor(&deq1, MyClass_less_by_id);
            Deque_MyClass_ctor(&deq2, MyClass_less_by_id);

            deq1.push_back(&deq1, MyClass{1, "Joe"});
            deq1.push_back(&deq1, MyClass{2, "Jane"});
            deq1.push_back(&deq1, MyClass{3, "Mary"});
            deq2.push_back(&deq2, MyClass{1, "Joe"});
            deq2.push_back(&deq2, MyClass{2, "Jane"});
            deq2.push_back(&deq2, MyClass{3, "Mary"});

            assert(Deque_MyClass_equal(deq1, deq2));

            deq1.pop_back(&deq1);
            assert(!Deque_MyClass_equal(deq1, deq2));
            deq1.push_back(&deq1, MyClass{4, "Mary"});
            assert(!Deque_MyClass_equal(deq1, deq2));

            deq1.dtor(&deq1);
            deq2.dtor(&deq2);
        }

      /*
      // Test performance.
      {
            std::default_random_engine e;
            using rand = std::uniform_int_distribution<int>;
            std::uniform_real_distribution<float> action;
            Deque_int deq;
            Deque_int_ctor(&deq, int_less);

            for (int i = 0; i < 10000; i++) deq.push_back(&deq, i);

            // In one end, out the other.
            for (int i = 0; i < 20000000; i++) 
            {
                  // fprintf(stderr, "iter: %d\n", i);
                  deq.push_back(&deq, i);
                  deq.pop_front(&deq);
            }
        
            for (int i = 0; i < 20000000; i++) 
            {
                  deq.push_front(&deq, i);
                  deq.pop_back(&deq);
            }

             // To do some computation, to prevent compiler from optimizing out.
            size_t sum = 0, max_size = 0;
        
            // Random.
            int pf = 0, pb = 0, pof = 0, pob = 0;
            for (int i = 0; i < 10000000; i++) 
            {
                  if (action(e) > .55) 
                  {
                        if (rand(0, 1)(e) == 0) 
                        {
                              deq.push_back(&deq, i);
                              pf++;
                        } 
                        else 
                        {
                              deq.push_front(&deq, i);
                              pb++;
                        }
                  } 
                  else 
                  {
                        if (deq.size(&deq) > 0) 
                        {
                              if (rand(0, 1)(e) == 0) 
                              {
                                    deq.pop_back(&deq);
                                    pob++;
                              } 
                              else 
                              {
                                    deq.pop_front(&deq);
                                    pof++;
                              }
                        }
                  }
            
                  max_size = std::max(deq.size(&deq), max_size);
                  // Randomly access one element.
            
                  if (deq.size(&deq) > 0) sum += deq.at(&deq, rand(0, deq.size(&deq) - 1)(e));
            }
        
            FILE * devnull = fopen("/dev/null", "w");
            assert(devnull != 0);

            // Print it out to prevent optimizer from optimizing out the at() calls.
            fprintf(devnull, "%lu", sum);
            printf("%zu max size\n", max_size);
            printf("%d push_backs, %d push_fronts, %d pop_backs, %d pop_fronts, %d size\n", 
                        pb, pf, pob, pof, (int) deq.size(&deq));
            deq.dtor(&deq);
      }
      */

      // Test random access performanc
      /*
      {
            size_t sum = 0;
            int lo = 0, hi = 1000000;
            Deque_int deq;
            Deque_int_ctor(&deq, int_less);

            for(int i = lo; i < hi; i++) deq.push_back(&deq, i);

            for(int i = lo; i < hi; i++) sum += deq.at(&deq, i);
       
            printf("Sum of all integers between %d and %d calculated using a deque is %lu.\n", lo, hi, sum);
            deq.dtor(&deq);
      }
      */

      // Test sort.
      // You must be able to work with the correct less-than function.
      {
        Deque_int deq;
        Deque_int_ctor(&deq, int_less);
        std::default_random_engine e;
        using rand = std::uniform_int_distribution<int>;

        for (int i = 0; i < 10000; i++) {
            deq.push_back(&deq, rand(-1000000, 1000000)(e));
        }

        deq.sort(&deq, deq.begin(&deq), deq.end(&deq));

        deq.dtor(&deq);
      }


      // Sorting Test 2
      {
       Deque_int deq1;
       Deque_int_ctor(&deq1, int_less);

       for (int i = 0; i < 10000; i++) 
       {
           deq1.push_back(&deq1, i);
       }

       for (int i = 20000; i >= 10000; i--) 
       {
           deq1.push_back(&deq1, i);
       }

       deq1.push_back(&deq1, 20001);
      
       auto iter1 =  deq1.end(&deq1);
       iter1.dec(&iter1);     
       
       auto iter2 = deq1.begin(&deq1);
       
       for (int i = 0; i < 10000; i++) 
       {
           iter2.inc(&iter2);
       }

       deq1.sort(&deq1, iter2, iter1);

       Deque_int deq2;
       Deque_int_ctor(&deq2, int_less);

       for(int i = 0; i <= 20001; i++) 
       {
           deq2.push_back(&deq2, i);
       }

       assert(Deque_int_equal(deq1, deq2)); 

       deq1.dtor(&deq1);
       deq2.dtor(&deq2);
      }

      // Test sort with different comparators.
      {
        Deque_MyClass sort_by_id, sorted_by_id;
        Deque_MyClass sort_by_name, sorted_by_name;

        Deque_MyClass_ctor(&sort_by_id, MyClass_less_by_id);
        Deque_MyClass_ctor(&sorted_by_id, MyClass_less_by_id);
        Deque_MyClass_ctor(&sort_by_name, MyClass_less_by_name);
        Deque_MyClass_ctor(&sorted_by_name, MyClass_less_by_name);

        sort_by_id.push_back(&sort_by_id, MyClass{1, "Bob"});
        sort_by_id.push_back(&sort_by_id, MyClass{3, "Sheldon"});
        sort_by_id.push_back(&sort_by_id, MyClass{2, "Alex"});

        sorted_by_id.push_back(&sorted_by_id, MyClass{1, "Bob"});
        sorted_by_id.push_back(&sorted_by_id, MyClass{2, "Alex"});
        sorted_by_id.push_back(&sorted_by_id, MyClass{3, "Sheldon"});

        sort_by_name.push_back(&sort_by_name, MyClass{1, "Bob"});
        sort_by_name.push_back(&sort_by_name, MyClass{3, "Sheldon"});
        sort_by_name.push_back(&sort_by_name, MyClass{2, "Alex"});

        sorted_by_name.push_back(&sorted_by_name, MyClass{2, "Alex"});
        sorted_by_name.push_back(&sorted_by_name, MyClass{1, "Bob"});
        sorted_by_name.push_back(&sorted_by_name, MyClass{3, "Sheldon"});

        assert(!Deque_MyClass_equal(sort_by_id, sorted_by_id));
        sort_by_id.sort(&sort_by_id, sort_by_id.begin(&sort_by_id), sort_by_id.end(&sort_by_id));
        assert(Deque_MyClass_equal(sort_by_id, sorted_by_id));

        assert(!Deque_MyClass_equal(sort_by_name, sorted_by_name));
        sort_by_name.sort(&sort_by_name, sort_by_name.begin(&sort_by_name), sort_by_name.end(&sort_by_name));
        assert(Deque_MyClass_equal(sort_by_name, sorted_by_name));

        sort_by_id.dtor(&sort_by_id);
        sorted_by_id.dtor(&sorted_by_id);
        sort_by_name.dtor(&sort_by_name);
        sorted_by_name.dtor(&sorted_by_name);
      }
}
