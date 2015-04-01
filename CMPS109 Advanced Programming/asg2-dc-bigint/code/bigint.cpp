/*
Evan Hughes [eshughes@ucsc.edu]
Hadley Black [hablack@ucsc.edu]
Assignment 2 - bigint dc
*/
// $Id: bigint.cpp,v 1.57 2014-04-25 15:17:53-07 - - $

#include <cassert>
#include <cstdlib>
#include <exception>
#include <limits>
#include <stack>
#include <stdexcept>
#include <string>
#include <cmath>
#include "bigint.h"
#include "debug.h"

using namespace std;

typedef unsigned char digit_t;
typedef vector<digit_t> bigvalue_t;

//
// constructor: creates an empty bigint 
//
bigint::bigint() {
   
   negative = false; 
}

//
// copy constructor
//
bigint::bigint (const bigint& that) {
  big_value = that.big_value;
  negative = that.negative;
  // CDTOR_TRACE;
}


bigint& bigint::operator= (const bigint& that) {
   if (this == &that) return *this;
   negative = that.negative;
   big_value = that.big_value;
   return *this;
}

bigint::~bigint() {
   //CDTOR_TRACE;
}

//construct from a long
bigint::bigint (long that) {
   bigint new_b_i = bigint(to_string(that));
   //CDTOR_TRACE;
}

//Construct from a string
bigint::bigint (const string& that) {
   assert (that.size() > 0);
   negative = false; // set to posative by default 
   int itor = that.size() - 1;
   while (itor >= 0) {
      if (that[itor] == '_'|| that[itor] == '-') { 
         this->negative = true; break;
      }

      this->big_value.push_back(that[itor]);
      --itor;
   }
   //long_value = isnegative ? - newval : + newval;
   //CDTOR_TRACE;
}

//simple print for debugging
void print(const bigvalue_t& num) {
   for(int i = num.size() - 1; i >= 0; --i) {
      cout << num[i];
   } 
}

bigint bigint::operator+ (const bigint& that) const {
   bigint result;
   if(this->negative == that.negative) { 
      result.big_value = do_bigadd(this->big_value, that.big_value);
      result.set_sign(this->negative);
   } else {
      if(do_bigless(this->big_value, that.big_value)) {
         result.big_value = do_bigsub(that.big_value, this->big_value);
         result.negative = that.negative;   
      } else {
         result.big_value = do_bigsub(this->big_value, that.big_value);
         result.set_sign(this->negative);
      }
   }
   return result;
}

bigint bigint::operator- (const bigint& that) const {
   bigint result; 
   if(this->negative != that.negative) {
      result.big_value = do_bigadd(this->big_value, that.big_value);
      result.negative = this->negative;
   } else {
      if(do_bigless(this->big_value, that.big_value)) {
         result.big_value = do_bigsub(that.big_value, this->big_value);
         result.negative = not(that.negative);   
      } else {
        result.big_value = do_bigsub(this->big_value, that.big_value);
        result.negative = (this->negative);
      }
   }
   return result;
}

//
// WHAT IS THIS SUPPOSED TO DO???
//
bigint bigint::operator-() const {
   return *this;
}

//Converts a bigint to type long
long bigint::to_long() const {
   //max value of a long
   bigint min = bigint("9223372036854775807"); 
   if(!do_bigless(big_value, min.big_value)) {
      throw range_error("to_long: out of range");
   }
   long long_val = 0;
   for(size_t i = 0; i < big_value.size(); ++i) {
      long_val = long_val + ((big_value[i] - '0') * (pow(10,i)));
   }
   if(negative) long_val -= (long_val * 2);
   return long_val;
}



//
// Multiplication algorithm.
//
bigint bigint::operator* (const bigint& that) const {
   bigint result;
   if(this->negative == that.negative){
      result.negative = false;
   } else {
      result.negative = true;
   }
   result.big_value = do_bigmul(this->big_value, that.big_value); 
   return result;
}

// following algorithm in project description 
bigvalue_t bigint::do_bigmul(const bigvalue_t& left,
                             const bigvalue_t& right) const {
 
   bigvalue_t product;
   for(size_t i = 0; i < (left.size() + right.size()); i++){
      product.push_back('0');
   }
   unsigned int c, d; 
   for(size_t i = 0; i < left.size(); ++i) {
      c = 0;
      for(size_t j = 0; j < right.size(); ++j) {
         d = (product[i+j]-'0') + ((left[i]-'0') * (right[j]-'0')) + c;
         product[i+j] = (d % 10) + '0';
         c = d/10; // takes floor(d/10) by truncation like we want
      }
      product[i + right.size()] = (c + '0');
   }
   product = clear_zeros(product);
   return product;
}

//Multiply by 2 algorithm for long division
bigvalue_t bigint::mul_by_2 (bigvalue_t& big_value) const {
   bigint two;
   two.big_value.push_back('2');
   bigvalue_t result = do_bigmul(big_value, two.big_value); 
   return result;
}

//Division by 2 algorithm to aid with long division
bigvalue_t bigint::div_by_2 (bigvalue_t& big_value) const {
   bigvalue_t tmp = big_value;
   bigvalue_t size_one;
   size_one.push_back('1');
   bigvalue_t size_two;
   size_two.push_back('2');
   bigvalue_t pow_ten = bigint("1024").big_value;
   bigvalue_t pow_nine = bigint("512").big_value;
   bigvalue_t pow_eight = bigint("256").big_value;
   bigvalue_t quotient;
   quotient.push_back('0');

   for(;;) {
      bigvalue_t pow_big = pow_ten;
      bigvalue_t pow_mid = pow_nine;
      bigvalue_t pow_less = pow_eight;
      while(do_bigless(pow_big, tmp)) {
         pow_big = clear_zeros(pow_big);
         pow_mid = clear_zeros(pow_mid);
         pow_less = clear_zeros(pow_less);
         pow_big = mul_by_2(pow_big);
         pow_mid = mul_by_2(pow_mid);
         pow_less = mul_by_2(pow_less);
      }
      if(do_bigless(pow_mid, tmp)) {
         tmp = do_bigsub(tmp, pow_mid);
         quotient = do_bigadd(quotient, pow_less);
      } else if(do_bigless(size_one, tmp)) {
         tmp = do_bigsub(tmp, size_two);
         quotient = do_bigadd(quotient, size_one);
      }
      if(tmp.empty()){ break; }
      else if(tmp == size_one) { break; }       
   }
   return quotient;
}

//Long division algorithm
bigint::quotient_remainder bigint::divide (const bigint& that) const {
   bigvalue_t div = that.big_value;
   div = clear_zeros(div);
   if (div.empty()){ throw domain_error ("divide by 0"); }
   bigvalue_t zero;
   zero.push_back('0');
   bigvalue_t divisor = that.big_value;
   bigint quotient;
   quotient.big_value.push_back('0');
   bigint remainder; 
   remainder.big_value = this->big_value;
   bigvalue_t size_one;
   size_one.push_back('1');
   bigvalue_t power_of_2;
   power_of_2.push_back('1');
   while (do_bigless (divisor, remainder.big_value)) {
      divisor = mul_by_2 (divisor);
      power_of_2 = mul_by_2 (power_of_2);
   }
   while (do_bigless (zero, power_of_2)) {
      if (do_bigless (divisor, remainder.big_value)) {
         remainder.big_value = do_bigsub(remainder.big_value, divisor);
         quotient.big_value = do_bigadd(quotient.big_value, power_of_2);
      }
      divisor = div_by_2 (divisor);
      power_of_2 = div_by_2 (power_of_2);
   }
   if(remainder.big_value == div) {
      quotient.big_value = do_bigadd(quotient.big_value, size_one);
      remainder.big_value = do_bigsub(remainder.big_value, div);
   }
   return {quotient, remainder};
}


bigint bigint::operator/ (const bigint& that) const {
   return divide (that).first;
}

bigint bigint::operator% (const bigint& that) const {
   return divide (that).second;
}


bool bigint::operator== (const bigint& that) const {
   // check signs
   if(this->negative!=that.negative) return false;
   // check lengths
   if(this->big_value.size()!=that.big_value.size()) return false;
   // check digits
   for(size_t i = this->big_value.size(); i>0; --i) {
      if(this->big_value.at(i-1)!=that.big_value.at(i-1)){
         return false;
      }
   } 
   return true; 
}

bool bigint::operator< (const bigint& that) const {
   // check signs
   if(this->negative==that.negative) {
      if(!(this->negative)) {
         return do_bigless(this->big_value, that.big_value);  
      } else {           
         return !(do_bigless(this->big_value, that.big_value));  
      }                
   } else {
        // if signs are not the same: 
        if(this->negative)
           return true;
        else 
           return false; 
   }
   return false; 
}

ostream& operator<< (ostream& out, const bigint& that) {
   if(that.big_value.empty()){
      out << "0";
   } else {
      if(that.negative) cout << "-";
      for(size_t i = that.big_value.size(); i > 0; --i) {     
         out << that.big_value.at(i - 1);
      }
   }
   return out;
}

//Exponent algorithm
bigint pow (const bigint& base, const bigint& exponent) {
   DEBUGF ('^', "base = " << base << ", exponent = " << exponent);
   bigint zero = bigint("0");
   bigint one = bigint("1");
   zero = zero.zero_clear(zero); 
   if (base == zero){
      return zero;
   }
   bigint base_copy = base;
   long expt = exponent.to_long();
   bigint result = bigint("1");
   if (expt < 0) {
      base_copy = one / base_copy;
      expt = - expt;
   }
   while (expt > 0) {
      if (expt & 1) { //odd
         result = result * base_copy;
         --expt;
      } else { //even
         base_copy = base_copy * base_copy;
         expt /= 2;
      }
   }
   DEBUGF ('^', "result = " << result);
   return result;
}

// do_bigadd: adds left and right
bigvalue_t bigint::do_bigadd (const bigvalue_t& left, 
                              const bigvalue_t& right) const {
   
   bigvalue_t result;
   unsigned int l_dig, r_dig, sum, carry;
   carry = 0;
   // note: iterate 1 more time then size to account for carry and 
   //       assign 0's if necessary 
   for(size_t i = 0; 
       i <= max(left.size(), right.size()); 
       ++i) {
      // reset sum
      sum = 0;
      // get digits
      if(i < left.size())
         l_dig = left[i] - '0';
      else 
         l_dig = 0;
      if(i < right.size()) 
         r_dig = right[i] - '0';
      else 
         r_dig = 0;
      // add digits (including carry from last iteration) 
      sum = l_dig + r_dig + carry;
      carry = 0;
      if(sum > 9) { 
         sum -= 10; carry = 1; 
      }
      result.push_back(sum + '0'); // make sure this is a char somehow?
   }
   result = clear_zeros(result);
   return result; 
}

// do_bigsub: subtracts right from left
bigvalue_t bigint::do_bigsub (const bigvalue_t& left, 
                              const bigvalue_t& right) const {
   bigvalue_t result;
   int l_dig, r_dig, diff, borrow;
   borrow = 0;
   for(size_t i = 0; 
       i < max(left.size(), right.size()); 
       ++i) {
      // reset diff
      diff = 0;
      // get digits
      if(i < left.size()) 
         l_dig = left[i] - '0';
      else 
         l_dig = 0;
      if(i < right.size()) 
         r_dig = right[i] - '0';
      else 
         r_dig = 0;

      // subtract digits (including borrow from last iteration) 
      diff = l_dig - r_dig + borrow;
      borrow = 0;
      if(diff < 0) { diff += 10; borrow = -1; }
         result.push_back(diff + '0');
   }
   result = clear_zeros(result);
   return result; 
}

// do_bigless: compares abs value of the 2 elements
// returns true if left < right
bool bigint::do_bigless(const bigvalue_t& left, 
                        const bigvalue_t& right) const { 

   if(left.size() < right.size()) return true; 
   if(left.size() > right.size()) return false;
     for(size_t i = left.size(); i > 0; --i) {     
       if(left.at(i-1) < right.at(i-1)) return true;  
       if(left.at(i-1) > right.at(i-1)) return false; 
     }
   return false;  
}

// clear_zeros(): clears leading 0's
bigvalue_t bigint::clear_zeros (bigvalue_t big_num) const {
   for(size_t i = big_num.size(); i > 0; --i) {
      if(big_num[i-1] != '0') break;
      else big_num.pop_back();
   }

   return big_num;
}

bigint bigint::zero_clear(bigint big) const{
   for(size_t i = big.big_value.size(); i > 0; --i) {
      if(big.big_value[i-1] != '0') 
         break;
      else 
         big.big_value.pop_back();
   }
   return big;
}
