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

struct Deque_int_Iterator
{
      int * iter;
      int index;
      int deq_capacity;

      int * first_data;
      int * last_data;

      void (* inc) (Deque_int_Iterator *);
      void (* dec) (Deque_int_Iterator *);
      int & (* deref) (Deque_int_Iterator *);
};

int & deref_def(Deque_int_Iterator * deq_iter_ptr)
{
	return *(deq_iter_ptr -> iter);
}

void inc_def(Deque_int_Iterator * deq_iter_ptr)
{
      deq_iter_ptr -> index++;

      if (deq_iter_ptr -> index == deq_iter_ptr -> deq_capacity)
      {
            deq_iter_ptr -> index = 0;
            deq_iter_ptr -> iter = deq_iter_ptr -> first_data;
      }
      else if (deq_iter_ptr -> index > deq_iter_ptr -> deq_capacity) assert(false);
      else deq_iter_ptr -> iter++;
}

void dec_def(Deque_int_Iterator * deq_iter_ptr)
{
      deq_iter_ptr -> index--;

      if (deq_iter_ptr -> index == -1)
      {
            deq_iter_ptr -> index = deq_iter_ptr -> deq_capacity;
            deq_iter_ptr -> iter = deq_iter_ptr -> last_data;
      }
      else if (deq_iter_ptr -> index < -1) assert(false);
      else deq_iter_ptr -> iter--;
}

bool
Deque_int_Iterator_equal(Deque_int_Iterator it, Deque_int_Iterator end)
{
	// Returns True if the iterators point to the same int object
      if (it.iter == end.iter) return true;
      else return false; 
}

struct Deque_int 
{
    	int * data;
    	int capacity;
    	size_t length;

    	int front_index;
    	int back_index;
    	char type_name[sizeof("Deque_int")];

    	size_t (* size) (Deque_int *);
    	bool (* empty) (Deque_int *);

    	void (* push_back) (Deque_int *, int);
    	void (* push_front) (Deque_int *, int);

      int & (* back) (Deque_int *);
      int & (* front) (Deque_int *);

      void (* pop_back) (Deque_int *);
      void (* pop_front) (Deque_int *);

      Deque_int_Iterator begin_iter;

      Deque_int_Iterator (* begin) (Deque_int *);
      Deque_int_Iterator (* end) (Deque_int *);

      void (* print) (Deque_int * );
      int & (* at) (Deque_int *, int );

      void (* clear) (Deque_int *);
      void (* dtor) (Deque_int *);

      void (* sort) (Deque_int *, Deque_int_Iterator, Deque_int_Iterator);
      bool (* comparator) (const int &, const int &);
};

size_t size_def(Deque_int * deq_ptr)
{
	return deq_ptr -> length;
}

bool empty_def(Deque_int * deq_ptr)
{
	// returns true is empty
	if (deq_ptr -> length) return false;
	else return true;
}

bool resize(Deque_int * deq_ptr)
{
      if ((int) deq_ptr -> length == deq_ptr -> capacity - 2)
      {
            deq_ptr -> capacity *= 2;
            deq_ptr -> data = (int *) realloc(deq_ptr -> data, deq_ptr -> capacity * sizeof(int));
            return true;
      }
      else if ((int) deq_ptr -> length >= deq_ptr -> capacity - 1) assert(false);

      return false;
}

void rearrange(Deque_int * deq_ptr)
{
      int old_capacity = deq_ptr -> capacity / 2;

      /* if back index did not wrap around, no need to rearrage */
      // if (deq_ptr -> back_index > old_capacity / 2) return;
      if (deq_ptr -> back_index > deq_ptr -> front_index) return;

      int num_data_to_move = old_capacity - deq_ptr -> front_index - 1;
      int dist_to_beg = old_capacity - num_data_to_move;

      for (int i = 0; i < num_data_to_move; i++)
      {
            deq_ptr -> data[dist_to_beg + old_capacity + i] = deq_ptr -> data[dist_to_beg + i];
      }

      deq_ptr -> front_index += old_capacity;
}

void push_back_def(Deque_int * deq_ptr, int obj)
{
      //printf("pushing %d into [%d]\n", obj, deq_ptr -> back_index);
      deq_ptr -> length++;
      deq_ptr -> data[deq_ptr -> back_index] = obj;
      
      deq_ptr -> back_index = (deq_ptr -> back_index + 1) % deq_ptr -> capacity;

      bool resized = resize(deq_ptr);
      if (resized) rearrange(deq_ptr);
}

void push_front_def(Deque_int * deq_ptr, int obj)
{
      deq_ptr -> length++;
      deq_ptr -> data[deq_ptr -> front_index] = obj;

      deq_ptr -> front_index--;
      while (deq_ptr -> front_index < 0) deq_ptr -> front_index += deq_ptr -> capacity;

      bool resized = resize(deq_ptr);
      if (resized) rearrange(deq_ptr);
}

int & back_def(Deque_int * deq_ptr)
{
      int target = deq_ptr -> back_index - 1;
      while (target < 0) target += deq_ptr -> capacity;

      return deq_ptr -> data[target];
}

int & front_def(Deque_int * deq_ptr)
{
      return deq_ptr -> data[(deq_ptr -> front_index + 1) % deq_ptr -> capacity];
}

void pop_back_def(Deque_int * deq_ptr)
{
	// Should we remove the int object from the deq here ?
      deq_ptr -> back_index--;

      while (deq_ptr -> back_index < 0) deq_ptr -> back_index += deq_ptr -> capacity;
      deq_ptr -> length--;
}

void pop_front_def(Deque_int * deq_ptr)
{
	// Should we remove the int object from the deq here ?
      deq_ptr -> front_index = (deq_ptr -> front_index + 1) % deq_ptr -> capacity;
      deq_ptr -> length--;
}

void print_def(Deque_int * deq_ptr)
{
	printf("\nlength: %lu, capacity: %d, front index: %d, back index: %d\n", 
            deq_ptr -> length, deq_ptr -> capacity, deq_ptr -> front_index, deq_ptr -> back_index);

      for (int i = 0; i < (int) deq_ptr -> capacity; i++)
      {
            printf("%d ", deq_ptr -> data[i]);
      }

	printf("\n\n");
}

Deque_int_Iterator begin_def(Deque_int * deq_ptr)
{
      int target = (deq_ptr -> front_index + 1) % deq_ptr -> capacity;

	(deq_ptr -> begin_iter).iter = &(deq_ptr -> data[target]);
      (deq_ptr -> begin_iter).index = target;

      (deq_ptr -> begin_iter).first_data = &(deq_ptr -> data[0]);
      (deq_ptr -> begin_iter).last_data = &(deq_ptr -> data[deq_ptr -> capacity - 1]);

      (deq_ptr -> begin_iter).deq_capacity = deq_ptr -> capacity;

      return deq_ptr -> begin_iter;
}

Deque_int_Iterator end_def(Deque_int * deq_ptr)
{
	(deq_ptr -> begin_iter).iter = &(deq_ptr -> data[deq_ptr -> back_index]);
      (deq_ptr -> begin_iter).index = deq_ptr -> back_index;

      (deq_ptr -> begin_iter).first_data = &(deq_ptr -> data[0]);
      (deq_ptr -> begin_iter).last_data = &(deq_ptr -> data[deq_ptr -> capacity - 1]);

      (deq_ptr -> begin_iter).deq_capacity = deq_ptr -> capacity;

      return deq_ptr -> begin_iter;
}

int & at_def(Deque_int * deq_ptr, int index)
{
      int target = deq_ptr -> front_index + 1 + index;
      while (target >= (int) deq_ptr -> capacity) target -= deq_ptr -> capacity; 

	return deq_ptr -> data[target];
}

void clear_def(Deque_int * deq_ptr)
{
	// Should I remove the elements here ? if so what should be the behavior ?
	deq_ptr -> length = 0;
	deq_ptr -> back_index = deq_ptr -> capacity / 2;
	deq_ptr -> front_index = deq_ptr -> back_index - 1;
}

void dtor_def(Deque_int * deq_ptr)
{
	free(deq_ptr -> data);
}

void sort_swap(int * i, int * j)
{
      int tmp = *i;
      *i = *j;
      *j = tmp;
}

Deque_int_Iterator sort_partition(Deque_int * deq, Deque_int_Iterator low, Deque_int_Iterator high)
{
      int * pivot = high.iter;
      
      Deque_int_Iterator i;
      i.iter = low.iter - 1;

      for (int * j = low.iter; j <= high.iter - 1; j++)
      {
            // returns True if first obj is smaller
            if (deq -> comparator(*j, *pivot))
            {
                  i.iter++;
                  sort_swap(i.iter, j);
            }
      }

      i.iter++;
      sort_swap(i.iter, high.iter);

      //printf("partition returns: %d\n", *i.iter);
      return i;
}

void sort_orig(Deque_int * deq_ptr, Deque_int_Iterator beg, Deque_int_Iterator end)
{
      if (beg.iter < end.iter)
      {
            Deque_int_Iterator pi = sort_partition(deq_ptr, beg, end);

            pi.iter--;
            sort_orig(deq_ptr, beg, pi);

            pi.iter += 2;
            sort_orig(deq_ptr, pi, end);
      }
}

void iter_dec(Deque_int_Iterator * deq_iter_ptr)
{
      deq_iter_ptr -> index--;

      if (deq_iter_ptr -> index == -1)
      {
            deq_iter_ptr -> index = deq_iter_ptr -> deq_capacity;
            deq_iter_ptr -> iter = deq_iter_ptr -> last_data;
      }
      else if (deq_iter_ptr -> index < -1) assert(false);
      else deq_iter_ptr -> iter--;
}

void sort_def(Deque_int * deq_ptr, Deque_int_Iterator beg, Deque_int_Iterator end)
{
      iter_dec(&end); /* Because end iterator is end + 1 (back_index) */

      if (end.index > beg.index) /* data is ordered */
      {
            if (beg.iter < end.iter)
            {
                  Deque_int_Iterator pi = sort_partition(deq_ptr, beg, end);

                  pi.iter--;
                  sort_orig(deq_ptr, beg, pi);

                  pi.iter += 2;
                  sort_orig(deq_ptr, pi, end);
            }
      }
      else /* data is wrapped around */
      {
            //int data_tmp[deq_ptr -> capacity];
            int * data_tmp = (int *) malloc(deq_ptr -> capacity * sizeof(int));

            for (int i = 0; i < (int) deq_ptr -> length; i++)
            {
                  data_tmp[i] = deq_ptr -> data[(deq_ptr -> front_index + 1 + i) % deq_ptr -> capacity];
            }

            for (int i = 0; i < (int) deq_ptr -> length; i++) 
            {
                  deq_ptr -> data[i + 1] = data_tmp[i];
            }

            deq_ptr -> front_index = 0;
            deq_ptr -> back_index = deq_ptr -> length + 1;
            
            Deque_int_Iterator beg_new = deq_ptr -> begin(deq_ptr);
            Deque_int_Iterator end_new = deq_ptr -> end(deq_ptr);
            iter_dec(&end_new);

            if (beg_new.iter < end_new.iter)
            {
                  Deque_int_Iterator pi = sort_partition(deq_ptr, beg_new, end_new);

                  pi.iter--;
                  sort_orig(deq_ptr, beg_new, pi);

                  pi.iter += 2;
                  sort_orig(deq_ptr, pi, end_new);
            }

            free(data_tmp);
      }
}

void 
Deque_int_ctor(Deque_int * deq_ptr, bool (* cmp) (const int &, const int &))
{
      // That will call malloc in test.cpp
      deq_ptr -> capacity = 5;
	deq_ptr -> data = (int *) malloc(deq_ptr -> capacity * sizeof(int));
      
	deq_ptr -> length = 0;
	deq_ptr -> back_index = deq_ptr -> capacity / 2;
	deq_ptr -> front_index = deq_ptr -> back_index - 1;
	strcpy(deq_ptr -> type_name, "Deque_int");

	deq_ptr -> size = size_def;
	deq_ptr -> empty = empty_def;

	deq_ptr -> push_back = push_back_def;
	deq_ptr -> push_front = push_front_def;

      deq_ptr -> back = back_def;
      deq_ptr -> front = front_def;

      deq_ptr -> pop_back = pop_back_def;
      deq_ptr -> pop_front = pop_front_def;

      deq_ptr -> begin = begin_def;
      deq_ptr -> end = end_def;

      (deq_ptr -> begin_iter).inc = inc_def;
      (deq_ptr -> begin_iter).dec = dec_def;
      (deq_ptr -> begin_iter).deref = deref_def;
      (deq_ptr -> begin_iter).deq_capacity = deq_ptr -> capacity;

      deq_ptr -> print = print_def;
      deq_ptr -> at = at_def;

      deq_ptr -> clear = clear_def;
      deq_ptr -> dtor = dtor_def;

      deq_ptr -> sort = sort_def;
      deq_ptr -> comparator = cmp;
}

bool
Deque_int_equal(Deque_int deq1, Deque_int deq2)
{
      // Returns true if deqs are same length and all elements are equal
      if (deq1.length != deq2.length) return false;

      for (size_t i = 0; i < deq1.length; i++)                                                  
      {                                                                                         
            int a = deq1.data[(i + deq1.front_index + 1) % deq1.capacity];                                       
            int b = deq2.data[(i + deq2.front_index + 1) % deq2.capacity];                                                 

            if (deq1.comparator(a, b)) return false;                                            
            if (deq1.comparator(b, a)) return false;                                            
      }                                                                                         

      return true;
}

bool
int_less (const int & o1, const int & o2) 
{
    return o1 < o2;
}

int main() 
{
      // Test that it can handle other types.  Tests are the same, more or less.
      /*
      {
        Deque_int deq;
        Deque_int_ctor(&deq, int_less);

        assert(deq.size(&deq) == 0);
        assert(deq.empty(&deq));

        // Should print "---- Deque_int, 10".
        //printf("---- %s, %d\n", deq.type_name, int(sizeof(deq.type_name)));
        // std::cout << "---- " << deq.type_name << ", " << sizeof(deq.type_name) << std::endl;
        assert(sizeof deq.type_name == 10);

        //printf("init:");
        //deq.print(&deq);

        deq.push_back(&deq, 10);
        //printf("added 1");
        //deq.print(&deq);

        deq.push_back(&deq, 20);
        //printf("added 2");
        //deq.print(&deq);

        deq.push_back(&deq, 30);
        //printf("added 3");
        //deq.print(&deq);

        deq.push_front(&deq, -10);
        //printf("added -10 front");
        //deq.print(&deq);

        deq.push_front(&deq, -20);
        //printf("added -20 front");
        //deq.print(&deq);

        //printf("%d\n", deq.front(&deq));
        //printf("%d\n", deq.back(&deq));
        assert(deq.front(&deq) == -20);
        assert(deq.back(&deq) == 30);

        deq.pop_front(&deq);
        deq.pop_back(&deq);
        
        //deq.print(&deq);

        assert(deq.front(&deq) == -10);
        assert(deq.back(&deq) == 20);
        assert(deq.size(&deq) == 3);

        int counter = 0;

        for (Deque_int_Iterator it = deq.begin(&deq); !Deque_int_Iterator_equal(it, deq.end(&deq)); it.inc(&it)) 
        {
            counter++;
            printf("%d\n", it.deref(&it));

            if (counter > 20) break;
        }

        //printf("Test dec.\n");
        //deq.print(&deq);

        // Test decrement.
        {
            auto it = deq.end(&deq);

            //printf("%d\n", it.deref(&it));
            it.dec(&it);
            //printf("%d\n", it.deref(&it));

            assert(it.deref(&it) == 20);
        }

        // printf("Using at.\n");

        for (size_t i = 0; i < 3; i++) printf("%d: %d\n", int(i), deq.at(&deq, i));

        deq.clear(&deq);

        deq.dtor(&deq);
      }

      // Test equality.  It is undefined behavior if the two deques were constructed with different
      // comparison functions.
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

        //deq1.print(&deq1);
        //deq2.print(&deq2);
        
        assert(Deque_int_equal(deq1, deq2));

        deq1.pop_back(&deq1);
        assert(!Deque_int_equal(deq1, deq2));
        deq1.push_back(&deq1, 4);
        assert(!Deque_int_equal(deq1, deq2));

        deq1.dtor(&deq1);
        deq2.dtor(&deq2);
      }

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

      // Test random access performance
      {
            size_t sum = 0;

            int lo = 0, hi = 10000000;
            //int lo = 1, hi = 15;

            Deque_int deq;
            Deque_int_ctor(&deq, int_less);

            for(int i = lo; i < hi; i++) 
            {
                  //deq.print(&deq);
                  deq.push_back(&deq, i);
            }

            //deq.print(&deq);

            for(int i = lo; i < hi; i++) sum += deq.at(&deq, i);
            //for(int i = 0; i < hi - lo; i++) 
            {
                  //printf("%d \n", deq.at(&deq, i));
                  //sum += deq.at(&deq, i);
            }

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

            for (int i = 0; i < 10; i++) deq.push_back(&deq, rand(-1000000, 1000000)(e));
            //for (int i = 0; i < 10; i++) deq.push_back(&deq, i);

            deq.sort(&deq, deq.begin(&deq), deq.end(&deq));

            deq.dtor(&deq);
      }
      
      // Sorting Test 2
      {
       Deque_int deq1;
       Deque_int_ctor(&deq1, int_less);

       for (int i=0;i<10000;i++) {
           deq1.push_back(&deq1, i);
       }

       for (int i=20000;i>=10000;i--) {
           deq1.push_back(&deq1,i);
       }

       deq1.push_back(&deq1,20001);
      
       auto iter1 =  deq1.end(&deq1);
       iter1.dec(&iter1);   
       
       auto iter2 = deq1.begin(&deq1);
       
       for (int i=0;i<10000;i++) {
           iter2.inc(&iter2);
       }

       deq1.sort(&deq1, iter2,iter1);

       Deque_int deq2;
       Deque_int_ctor(&deq2 , int_less);

       for(int i=0;i<=20001;i++) {
           deq2.push_back(&deq2,i);
       }

       printf("before sort\n");
       assert(Deque_int_equal(deq1, deq2)); 
       printf("after sort\n");
      
        deq1.dtor(&deq1);
        deq2.dtor(&deq2);
     } 

     /*
      {
            Deque_int deq_sorted, deq2;
            Deque_int_ctor(&deq_sorted, int_less);
            Deque_int_ctor(&deq2, int_less);

            for (int i = 0; i < 3; i++) deq_sorted.push_back(&deq_sorted, i);
            
            deq2.push_back(&deq2, 1);
            deq2.push_back(&deq2, 2);
            deq2.push_back(&deq2, 0);

            printf("\nsorted array:");
            deq_sorted.print(&deq_sorted);

            printf("not sorted array:");
            deq2.print(&deq2);

            deq2.sort(&deq2, deq2.begin(&deq2), deq2.end(&deq2));

            printf("supposedly sorted array:");
            deq2.print(&deq2);

            assert(Deque_int_equal(deq_sorted, deq2)); 
      }

      {
      
       Deque_int deq1;
       Deque_int_ctor(&deq1, int_less);

        std::default_random_engine e;
        using rand = std::uniform_int_distribution<int>;

        for (int i = 0; i < 1000000; i++) {
            deq1.push_back(&deq1, rand(-1000000, 1000000)(e));
        }

       auto iter1 = deq1.begin(&deq1);
       auto iter2 = deq1.begin(&deq1);

       for(int i=0;i<10;i++)
           iter1.inc(&iter1);

       for(int i=0;i<20;i++)
           iter2.inc(&iter2);

       for(int i=0;i<1000000;i++)
               deq1.sort(&deq1, iter1,iter2);

       deq1.dtor(&deq1);

      }
      */
}
