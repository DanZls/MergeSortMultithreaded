# pragma once

#include <vector>
#include <future>

std::vector<std::vector<int>> Split(const std::vector<int>& v, int n = 2){
	std::vector<std::vector<int>> splits;
	std::vector<int>::const_iterator part_begin = v.begin();
	int v_size = v.size();
	while(n>0){
		int part_size = v_size/n;
		std::vector<int> part(part_begin, part_begin+part_size);
		splits.push_back(move(part));
		part_begin += part_size;
		v_size -= part_size;
		n--;
	}
	return splits;
}

std::vector<int> Merge(const std::vector<int>& v1, const std::vector<int>& v2){
	std::vector<int> merged;
	std::vector<int>::const_iterator it1 = v1.begin();
	std::vector<int>::const_iterator it2 = v2.begin();
	while(it1 != v1.end() && it2 != v2.end()){
		if(*it1 < *it2){
			merged.push_back(*it1);
			it1++;
		}
		else{
			merged.push_back(*it2);
			it2++;
		}
	}
	if(it1 == v1.end()) merged.insert(merged.end(), it2, v2.end());
	if(it2 == v2.end()) merged.insert(merged.end(), it1, v1.end());
	return merged;
}

std::vector<int> MergeSort(const std::vector<int>& v){
	if(v.size() <= 1) return v;
	std::vector<std::vector<int>> splits = Split(v);
	return Merge(MergeSort(splits[0]), MergeSort(splits[1]));
}

std::vector<int> ThreadedMergeSort(const std::vector<int>& v, int threads = 16){
	auto s = Split(v, threads);
	std::vector<std::future<std::vector<int>>> f;
	for(int i = 0; i<threads; i++){
		f.push_back(std::async(MergeSort, s[i]));
	}
	std::vector<std::vector<int>> m;
	for(int i = 0; i<threads; i++){
		m.push_back(f[i].get());
	}
	while(m.size()>1){
		std::vector<std::vector<int>> mn;
		for(size_t i = 0; i < m.size(); i += 2){
			mn.push_back(Merge(m[i], m[i+1]));
		}
		m = move(mn);
	}
	return m[0];
}
