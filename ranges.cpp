#include <vector>
#include <iostream>

namespace iterator
{
   template<class BaseIterator, class Predicate>
   struct filtered
   {
   public:
      filtered(BaseIterator it, BaseIterator end, Predicate pred)
         : base_it_(it)
         , base_end_(end)
         , pred_(pred)
      {
         advance_if_needed();
      }

      filtered& operator++ ()
      {
         ++base_it_;
         advance_if_needed();
         return *this;
      }

      auto operator * () const
      {
         return *base_it_;
      }

      friend bool operator == (filtered const & a, filtered const & b)
      {
         return a.base_it_ == b.base_it_;
      }

      friend bool operator != (filtered const & a, filtered const & b)
      {
         return !(a == b);
      }

   private:
      void advance_if_needed()
      {
         for (; base_it_ != base_end_; ++base_it_)
         {
            if (pred_(*base_it_))
               break;
         }
      }

   private:
      BaseIterator base_it_, base_end_;
      Predicate pred_;
   };

   template<class BaseIterator, class Functor>
   struct transformed
   {
      transformed(BaseIterator it, Functor func)
         : base_it_(it)
         , func_(func)
      {}

      transformed& operator++ ()
      {
         ++base_it_;
         return *this;
      }

      auto operator * () const
      {
         return func_(*base_it_);
      }

      friend bool operator == (transformed const & a, transformed const & b)
      {
         return a.base_it_ == b.base_it_;
      }

      friend bool operator != (transformed const & a, transformed const & b)
      {
         return !(a == b);
      }

   private:
      BaseIterator base_it_;
      Functor func_;
   };


   template<class BaseIterator, class Predicate>
   filtered<BaseIterator, Predicate> make_filtered(BaseIterator it, BaseIterator end, Predicate pred)
   {
      return { it, end, pred };
   }

   template<class BaseIterator, class Functor>
   transformed<BaseIterator, Functor> make_transformed(BaseIterator it, Functor func)
   {
      return { it, func };
   }
}

namespace range
{
   template<class BaseRange, class Predicate>
   struct filtered
   {
      filtered(BaseRange const & range, Predicate pred)
         : base_range_(range)
         , pred_(pred)
      {}

      auto begin() const
      {
         return iterator::make_filtered(base_range_.begin(), base_range_.end(), pred_);
      }

      auto end() const
      {
         return iterator::make_filtered(base_range_.end(), base_range_.end(), pred_);
      }

   private:
      BaseRange const & base_range_;
      Predicate pred_;
   };

   template<class BaseRange, class Functor>
   struct transformed
   {
      transformed(BaseRange const & range, Functor func)
         : base_range_(range)
         , func_(func)
      {}

      auto begin() const
      {
         return iterator::make_transformed(base_range_.begin(), func_);
      }

      auto end() const
      {
         return iterator::make_transformed(base_range_.end(), func_);
      }

   private:
      BaseRange const & base_range_;
      Functor func_;
   };

   namespace adaptors
   {
      template<class Predicate>
      struct _filtered
      {
         Predicate predicate;
      };

      template<class Predicate>
      _filtered<Predicate> filtered(Predicate pred)
      {
         return { pred };
      }

      template<class BaseRange, class Predicate>
      range::filtered<BaseRange, Predicate> operator | (BaseRange const & range, _filtered<Predicate> const & adaptor)
      {
         return { range, adaptor.predicate };
      }

      template<class Functor>
      struct _transformed
      {
         Functor functor;
      };

      template<class Functor>
      _transformed<Functor> transformed(Functor func)
      {
         return { func };
      }

      template<class BaseRange, class Functor>
      range::transformed<BaseRange, Functor> operator | (BaseRange const & range, _transformed<Functor> const & adaptor)
      {
         return { range, adaptor.functor };
      }
   }
}

bool is_even(int x) { return x % 2 == 0; }

int sqr(int x) { return x * x; }

int main()
{
   using namespace range::adaptors;

   std::vector<int> xs = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

   int sum = 0;
   for (int x : xs | filtered(&is_even) | transformed(&sqr))
      sum += x;

   std::cout << sum << std::endl;
}
