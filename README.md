SORTING ALGORITHMS EXPLAINED

## **1. Bogo Sort**

**How it works:**

* Check if the array is sorted.
* If not, **shuffle all elements randomly**.
* Repeat until the array happens to be sorted.

**Time Complexity:**

* Average: **O(n!)**

---

## **2. Bubble Sort**

**How it works:**

* Repeatedly **compare adjacent elements** and swap if out of order.
* Largest elements “bubble” to the end.
* Repeat for all elements until sorted.

**Time Complexity:**

* Average: **O(n²)**

---

## **3. Cocktail Shaker Sort**

**How it works:**

* Similar to Bubble Sort, but goes **both directions**: left→right, then right→left.
* Moves **smallest elements to the beginning** and largest to the end in each pass.

**Time Complexity:**

* Average: **O(n²)**

---

## **4. Heap Sort**

**How it works:**

* Build a **max-heap** from the array.
* Repeatedly swap the root (largest element) with the last element, reduce heap size, and **heapify** the root.

**Time Complexity:**

* Average: **O(n log n)**

---

## **5. Merge Sort**

**How it works:**

* Divide the array into **two halves**.
* Recursively sort each half.
* **Merge the sorted halves** into a single sorted array.

**Time Complexity:**

* Average: **O(n log n)**

---

## **6. Quick Sort**

**How it works:**

* Pick a **pivot element** (commonly last element).
* Partition array: elements ≤ pivot go left, > pivot go right.
* Recursively sort left and right subarrays.

**Time Complexity:**

* Average: **O(n log n)**

---

## **7. Radix Sort**

**How it works:**

* Sort numbers **digit by digit**, usually from **least significant digit** to **most significant**.
* Use **counting sort** or bucket sort for each digit.

**Time Complexity:**

* Average: **O(d * n)**, where d = number of digits

---

### **Summary Table**

| Algorithm            | Time Complexity | Space    | 
| -------------------- | --------------- | -------- | 
| Bogo Sort            | O(n!)           | O(1)     |
| Bubble Sort          | O(n²)           | O(1)     | 
| Cocktail Shaker Sort | O(n²)           | O(1)     | 
| Heap Sort            | O(n log n)      | O(1)     | 
| Merge Sort           | O(n log n)      | O(n)     | 
| Quick Sort           | O(n log n) avg  | O(log n) |
| Radix Sort           | O(d * n)        | O(n + k) |


