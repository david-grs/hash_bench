Benchmark again different hash map implementations. Not that many right now, more coming soon.

```
Test                                                                                                                                                            Time (ns)
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
insert: std::set<std::string, std::less<std::string>, std::allocator<std::string> >                                                                            43,710,190
insert: std::unordered_set<std::string, std::hash<std::string>, std::equal_to<std::string>, std::allocator<std::string> >                                      13,119,018
insert: google::dense_hash_set<std::string, std::tr1::hash<std::string>, std::equal_to<std::string>, google::libc_allocator_with_realloc<std::string> >        14,219,668
insert: hot_set<std::string, variable<std::string>, std::equal_to<void>, std::allocator<std::string>, std::hash<std::string>, default_load_policy>             11,642,394
insert: ht_chained<std::string>                                                                                                                                22,063,004
insert: rigtorp::HashMap<std::string, int, std::hash<std::string> >                                                                                            16,220,712

delete: std::set<std::string, std::less<std::string>, std::allocator<std::string> >                                                                            44,638,559
delete: std::unordered_set<std::string, std::hash<std::string>, std::equal_to<std::string>, std::allocator<std::string> >                                      18,031,331
delete: google::dense_hash_set<std::string, std::tr1::hash<std::string>, std::equal_to<std::string>, google::libc_allocator_with_realloc<std::string> >        12,614,844
delete: hot_set<std::string, variable<std::string>, std::equal_to<void>, std::allocator<std::string>, std::hash<std::string>, default_load_policy>             13,027,754
delete: rigtorp::HashMap<std::string, int, std::hash<std::string> >                                                                                            18,610,121
```

PS: std::set is not a hash, I know ; it is here for comparison purposes.

