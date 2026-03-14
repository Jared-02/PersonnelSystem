import hashlib

s = "2026-03-11-Jared-02"
s_bytes = s.encode('utf-8')

hash_object = hashlib.sha256(s_bytes)
print(f"{hash_object.hexdigest()}")
