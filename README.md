Benchmark again different hash map implementations. Not that many right now, more coming soon.

```
Test                                                                                                                                                    Time (ns)
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
std::set<std::string, std::less<std::string>, std::allocator<std::string> >                                                                            47,220,743
std::unordered_set<std::string, std::hash<std::string>, std::equal_to<std::string>, std::allocator<std::string> >                                      11,376,578
google::dense_hash_set<std::string, std::tr1::hash<std::string>, std::equal_to<std::string>, google::libc_allocator_with_realloc<std::string> >        10,509,684
hot_set<std::string, variable<std::string>, std::equal_to<void>, std::allocator<std::string>, std::hash<std::string>, default_load_policy>              8,920,329
ht_chained<std::string>                                                                                                                                14,842,823
```

PS: std::set is not a hash, I know ; it is here for comparison purposes.

