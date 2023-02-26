template<class T>
void quicksort(T data[], int first, int last) {
  int lower = first + 1, upper = last;
  swap(data[first], data[(first + last) / 2]);
  T bound = data[first];

  while (lower <= upper) {
    while (bound > data[lower])
      lower++;
    while (bound < data[upper])
      upper--;
    if (lower < upper)
      swap(data[lower++], data[upper--]);
    else lower++;
  }
  swap(data[upper], data[first]);
  if (first < upper - 1)
    quicksort(data, first, upper-1);
  if (upper + 1 < last)
    quicksort(data, upper + 1, last);
}

template<class T>
void quicksort(T data[], int n) {
  int i, max;
  if (n < 2)
    return;
  for (i = 1; max = 0; i < n; i++)
    if (data[max] < data[i])
      max = i;
  swap(data[n -1], data[max]);
  quicksort(data, 0, n - 2);
}