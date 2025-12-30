# Setting Password in pool_passwd File

The `pool_passwd` file stores encrypted passwords that pgpool2 uses to authenticate to the PostgreSQL backend server.

## Method 1: Using pg_md5 (Recommended)

This is the standard and easiest method:

### Interactive (prompts for password):
```bash
sudo pg_md5 -p -u postgres /etc/pgpool2/pool_passwd
# Enter password when prompted
```

### Non-interactive (specify password directly):
```bash
echo 'qazwsx' | sudo pg_md5 -p -u postgres /etc/pgpool2/pool_passwd
```

### Set proper permissions:
```bash
sudo chown postgres:postgres /etc/pgpool2/pool_passwd
sudo chmod 600 /etc/pgpool2/pool_passwd
```

### Verify the file:
```bash
sudo cat /etc/pgpool2/pool_passwd
```

Should show something like:
```
postgres:md5a1b2c3d4e5f6g7h8i9j0k1l2m3n4o5p6
```

## Method 2: Manual Creation

If `pg_md5` is not available, you can create it manually:

### Step 1: Generate MD5 hash
The format is: MD5(username + password)

```bash
# For username 'postgres' and password 'qazwsx'
echo -n 'postgresqazwsx' | md5sum
```

### Step 2: Create the file
```bash
# Replace HASH_HERE with the MD5 hash from step 1
sudo bash -c "echo 'postgres:md5HASH_HERE' > /etc/pgpool2/pool_passwd"
sudo chown postgres:postgres /etc/pgpool2/pool_passwd
sudo chmod 600 /etc/pgpool2/pool_passwd
```

## Method 3: Using Python

If neither `pg_md5` nor `md5sum` is available:

```bash
python3 -c "import hashlib; print('postgres:md5' + hashlib.md5(('postgres' + 'qazwsx').encode()).hexdigest())" | sudo tee /etc/pgpool2/pool_passwd
sudo chown postgres:postgres /etc/pgpool2/pool_passwd
sudo chmod 600 /etc/pgpool2/pool_passwd
```

## Complete Setup Example

For password 'qazwsx' (from your code):

```bash
# 1. Create/update pool_passwd file
echo 'qazwsx' | sudo pg_md5 -p -u postgres /etc/pgpool2/pool_passwd

# 2. Set permissions
sudo chown postgres:postgres /etc/pgpool2/pool_passwd
sudo chmod 600 /etc/pgpool2/pool_passwd

# 3. Verify
sudo cat /etc/pgpool2/pool_passwd

# 4. Make sure it's enabled in pgpool.conf
sudo grep '^pool_passwd' /etc/pgpool2/pgpool.conf
# Should show: pool_passwd = '/etc/pgpool2/pool_passwd'

# 5. If not enabled, enable it:
sudo sed -i "s|^#pool_passwd =|pool_passwd = '/etc/pgpool2/pool_passwd'|" /etc/pgpool2/pgpool.conf

# 6. Restart pgpool2
sudo systemctl restart pgpool2

# 7. Check status
sudo systemctl status pgpool2
```

## File Format

The `pool_passwd` file format is:
```
username:md5<md5_hash>
```

Where:
- `username` is the PostgreSQL username (e.g., `postgres`)
- `md5` is the literal string "md5"
- `<md5_hash>` is the MD5 hash of `username + password` concatenated

Example:
```
postgres:md5a1b2c3d4e5f6g7h8i9j0k1l2m3n4o5p6
```

## Important Notes

1. **File Permissions**: Must be readable only by postgres user (600)
2. **File Ownership**: Must be owned by postgres user
3. **Password Match**: The password in pool_passwd must match the PostgreSQL user's password
4. **Configuration**: Must be enabled in `pgpool.conf` with:
   ```conf
   pool_passwd = '/etc/pgpool2/pool_passwd'
   ```

## Troubleshooting

### If pg_md5 command not found:
```bash
# On Debian/Ubuntu
sudo apt-get install pgpool2

# Or use manual method (Method 2 or 3)
```

### If password doesn't work:
1. Verify PostgreSQL password is correct:
   ```bash
   PGPASSWORD=qazwsx psql -h 127.0.0.1 -p 5432 -U postgres -d postgres -c "SELECT 1;"
   ```

2. If PostgreSQL password is different, either:
   - Update pool_passwd with correct password
   - OR change PostgreSQL password to match:
     ```bash
     sudo -u postgres psql -c "ALTER USER postgres PASSWORD 'qazwsx';"
     ```

### Verify pool_passwd is working:
```bash
# Check if pgpool2 can authenticate
sudo systemctl restart pgpool2
sudo systemctl status pgpool2

# Test connection
psql -h 127.0.0.1 -p 9999 -U postgres -d postgres -c "SELECT version();"
```







