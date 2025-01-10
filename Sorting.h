#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

template <typename T>
class Sorting {
public:
	virtual std::vector<T> sort(const std::vector<T>&, const std::function<bool(const T&, const T&)>&) const = 0;
	virtual ~Sorting() = default;
};

template <typename T>
class CountingSorting final :
	public Sorting <T> {
public:
	std::vector<T> sort(const std::vector<T>& vec, const std::function<bool(const T&, const T&)>& comp) const override {
		if (vec.empty()) throw std::logic_error("Vector can't be empty");

		int sizeVec = vec.size();
		std::vector<int> count(sizeVec, 0);
		std::vector<T> result(sizeVec, 0);
		for (int i = 0; i < sizeVec - 1; ++i) {
			for (int j = i + 1; j < sizeVec; ++j) {
				if (comp(vec[i], vec[j])) ++count[j];
				else ++count[i];
			}
		}
		for (int i = 0; i < sizeVec; ++i)
			result[count[i]] = vec[i];
		return result;
	}
};

template<typename T>
class InsertionSorting final :
	public Sorting<T> {
public:
	std::vector<T> sort(const std::vector<T>& vec, const std::function<bool(const T&, const T&)>& comp) const override {
		if (vec.empty()) throw std::logic_error("Vector can't be empty");

		int sizeVec = vec.size();
		std::vector<T> result(vec);
		for (int i = 1; i < sizeVec; ++i) {
			int j = i - 1;
			T k = result[i];
			while (j >= 0 && comp(k, result[j])) {
				result[j + 1] = result[j];
				j--;
			}
			result[j + 1] = k;
		}
		return result;
	}
};
template<typename T>
class SelectionSorting final :
	public Sorting<T> {
public:
	std::vector<T> sort(const std::vector<T>& vec, const std::function<bool(const T&, const T&)>& comp) const override {
		if (vec.empty()) throw std::logic_error("Vector can't be empty");

		int sizeVec = vec.size();
		std::vector<T> result(vec);
		for (int i = 0; i < sizeVec - 1; ++i) {
			int minIndex = i;
			for (int j = i + 1; j < sizeVec; ++j) {
				if (comp(result[j], result[minIndex]))
					minIndex = j;
			}
			std::swap(result[i], result[minIndex]);
		}
		return result;
	}
};

template<typename T>
class ShellSorting final :
	public Sorting<T> {
public:
	std::vector<T> sort(const std::vector<T>& vec, const std::function<bool(const T&, const T&)>& comp) const override {
		if (vec.empty()) throw std::logic_error("Vector can't be empty");

		int sizeVec = vec.size();
		std::vector<T> result(vec);
		for (int h = sizeVec / 2; h > 0; h /= 2) {
			for (int i = h; i < sizeVec; ++i) {
				int j = i;
				T temp = result[i];
				while (j >= h && comp(temp, result[j - h])) {
					result[j] = result[j - h];
					j -= h;
				}
				result[j] = temp;
			}
		}
		return result;
	}
};

template<typename T>
class QuickSorting final :
	public Sorting<T> {
public:
	std::vector<T> sort(const std::vector<T>& vec, const std::function<bool(const T&, const T&)>& comp) const override {
		if (vec.empty()) throw std::logic_error("Vector can't be empty");

		std::vector<T> result(vec);
		quickSort(result, 0, result.size() - 1, comp);
		return result;
	}

private:
	int pivotPartition(std::vector<T>& vec, int start, int end, const std::function<bool(const T&, const T&)>& comp) const {
		T pivot = vec[end];
		int i = start - 1;

		for (int j = start; j < end; ++j) {
			if (comp(vec[j], pivot)) {
				++i;
				std::swap(vec[i], vec[j]);
			}
		}
		std::swap(vec[i + 1], vec[end]);
		return i + 1;
	}
	void quickSort(std::vector<T>& vec, int start, int end, const std::function<bool(const T&, const T&)>& comp) const {
		if (start < end) {
			int pi = pivotPartition(vec, start, end, comp);
			quickSort(vec, start, pi - 1, comp);
			quickSort(vec, pi + 1, end, comp);
		}
	}
};

template<typename T>
class HeapSorting final :
	public Sorting<T> {
public:
	std::vector<T> sort(const std::vector<T>& vec, const std::function<bool(const T&, const T&)>& comp) const override {
		if (vec.empty()) throw std::logic_error("Vector can't be empty");

		int sizeVec = vec.size();
		std::vector<T> result(vec);
		for (int i = sizeVec / 2 - 1; i >= 0; --i) {
			heapify(result, sizeVec,i, comp);
		}
		for (int i = sizeVec - 1; i > 0; --i) {
			std::swap(result[0], result[i]);
			heapify(result, i, 0, comp);
		}
		return result;
	}
private:
	void heapify(std::vector<T>& vec, int heapSize, int rootIndex, const std::function<bool(const T&, const T&)>& comp) const {
		int largest = rootIndex, leftChild = 2 * rootIndex + 1, rightChild = 2 * rootIndex + 2;

		if (leftChild < heapSize && comp(vec[largest], vec[leftChild])) {
			largest = leftChild;
		}
		if (rightChild < heapSize && comp(vec[largest], vec[rightChild])) {
			largest = rightChild;
		}
		if (largest != rootIndex) {
			std::swap(vec[rootIndex], vec[largest]);
			heapify(vec, heapSize, largest, comp);
		}
	}
};

template<typename T>
class RadixSorting final :
	public Sorting<T> {
public:
	std::vector<T> sort(const std::vector<T>& vec, const std::function<bool(const T&, const T&)>& comp) const override {
		if (vec.empty()) throw std::logic_error("Vector can't be empty");
		
		int sizeVec = vec.size();
		std::vector<T> result(vec);
		T maxElement = *std::max_element(result.begin(), result.end(), comp);
		int maxDigits = static_cast<int>(std::log10(maxElement)) + 1;

		for (int exp = 1; maxDigits > 0; exp *= 10, --maxDigits) {
			result = countSort(result, exp);
		}
		return result;
	}
private:
	std::vector<T> countSort(const std::vector<T>& vec, int exp) const {
		std::vector<T> output(vec.size());
		std::vector<int> count(10, 0);

		for (const T& elem : vec) {
			int digit = (elem / exp) % 10;
			++count[digit];
		}

		for (int i = 1; i < 10; ++i) {
			count[i] += count[i - 1];
		}

		for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
			int digit = (*it / exp) % 10;
			output[--count[digit]] = *it;
		}

		return output;
	}
};