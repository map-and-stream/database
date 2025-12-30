# Fixing pgpool2 SCRAM Authentication Error

## Error Message
```
failed to authenticate with backend using SCRAM
DETAIL: valid password not found
```

## Problem
pgpool2 is trying to connect to PostgreSQL backend but doesn't have the correct password stored. pgpool2 needs to authenticate to PostgreSQL on behalf of clients.

## Solutions

### Solution 1: Create pool_passwd File (Recommended)

The `pool_passwd` file stores encrypted passwords for pgpool2 to use when connecting to PostgreSQL.

#### Step 1: Generate pool_passwd file
```bash
sudo pg_md5 -p -u postgres /etc/pgpool2/pool_passwd
```
This will prompt you for the PostgreSQL password and create the encrypted password file.

#### Step 2: Set proper permissions
```bash
sudo chown postgres:postgres /etc/pgpool2/pool_passwd
sudo chmod 600 /etc/pgpool2/pool_passwd
```

#### Step 3: Enable pool_passwd in pgpool.conf
```bash
# Edit the file
sudo nano /etc/pgpool2/pgpool.conf

# Or use sed to uncomment/enable it
sudo sed -i 's/^#pool_passwd =/pool_passwd = \/etc\/pgpool2\/pool_passwd/' /etc/pgpool2/pgpool.conf
```

Make sure the line looks like:
```conf
pool_passwd = '/etc/pgpool2/pool_passwd'
```

#### Step 4: Restart pgpool2
```bash
sudo systemctl restart pgpool2
```

### Solution 2: Use .pgpass File

Create a `.pgpass` file in the postgres user's home directory:

```bash
# Format: hostname:port:database:username:password
sudo -u postgres bash -c 'echo "127.0.0.1:5432:*:postgres:YOUR_PASSWORD" > ~/.pgpass'
sudo -u postgres chmod 600 ~/.pgpass
```

Replace `YOUR_PASSWORD` with the actual PostgreSQL password.

### Solution 3: Change PostgreSQL Password

If you want to use the password from your code (`qazwsx`), change PostgreSQL password:

```bash
sudo -u postgres psql -c "ALTER USER postgres PASSWORD 'qazwsx';"
```

Then create the pool_passwd file with this password:
```bash
sudo pg_md5 -p -u postgres /etc/pgpool2/pool_passwd
# Enter: qazwsx
```

### Solution 4: Find Current PostgreSQL Password

If you don't know the current password, you can:

1. **Reset it using peer authentication** (if you have sudo access):
   ```bash
   sudo -u postgres psql -c "ALTER USER postgres PASSWORD 'qazwsx';"
   ```

2. **Check if you can connect without password** (peer/ident auth):
   ```bash
   sudo -u postgres psql -c "SELECT current_user;"
   ```

3. **Test the password from your code**:
   ```bash
   PGPASSWORD=qazwsx psql -h 127.0.0.1 -p 5432 -U postgres -d postgres -c "SELECT 1;"
   ```

## Verification

After fixing, test the connection:

```bash
# Test through pgpool2
psql -h 127.0.0.1 -p 9999 -U postgres -d postgres -c "SELECT version();"

# Or test your application
./your_application
```

## Additional Configuration

### Check pool_hba.conf
Make sure `/etc/pgpool2/pool_hba.conf` allows your connections:

```conf
host    all    all    127.0.0.1/32    md5
host    all    all    ::1/128         md5
```

### Check PostgreSQL pg_hba.conf
Make sure PostgreSQL allows connections from pgpool2:

```bash
sudo grep -E "^host" /etc/postgresql/16/main/pg_hba.conf
```

Should have entries like:
```conf
host    all    all    127.0.0.1/32    md5
host    all    all    ::1/128         md5
```

## Quick Fix Script

Run the provided script:
```bash
./fix_pgpool_auth.sh
```

This will show you the current status and provide commands to fix the issue.

## Common Issues

1. **pool_passwd file doesn't exist**: Create it using `pg_md5 -p`
2. **Wrong permissions**: Must be owned by postgres user and mode 600
3. **pool_passwd not enabled in config**: Uncomment the line in pgpool.conf
4. **Wrong password**: Make sure the password in pool_passwd matches PostgreSQL password
5. **SCRAM vs MD5**: PostgreSQL 10+ uses SCRAM by default. pool_passwd handles this automatically.

