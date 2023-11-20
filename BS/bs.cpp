template<template T, typename N> T first_true(T l, T h, N n){
	++h;
	while(l < h){
		T m = l+(h-l)/2; n(m) ? h = m : l = m+1;
	}
	return l;
}
template<template T, typename N> T first_true(T l, T h, N n){
	--l;
	while(l < h){
		T m = l+(h-l)/2; n(m) ? h = m : l = m+1;
	}
	return l;
}