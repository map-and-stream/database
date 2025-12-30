# pgpool2 Configuration Guide

## Error Analysis
The error `"failed to create a backend connection"` with `"executing failover on backend"` indicates that pgpool2 is running but cannot connect to the PostgreSQL backend server.

## Key Configuration Sections to Check

### 1. Backend Configuration (CRITICAL)
In `/etc/pgpool2/pgpool.conf`, check these settings:

```conf
# Backend connection settings
backend_hostname0 = 'localhost'        # or '127.0.0.1'
backend_port0 = 5432                   # PostgreSQL port
backend_weight0 = 1
backend_data_directory0 = '/var/lib/postgresql/16/main'
backend_flag0 = 'ALLOW_TO_FAILOVER'
```

**Common Issues:**
- `backend_hostname0` is wrong or not set
- `backend_port0` doesn't match your PostgreSQL port
- PostgreSQL is not running or not accessible from pgpool2

### 2. Network Configuration
```conf
listen_addresses = '*'          # or 'localhost' or '127.0.0.1'
port = 9999                      # pgpool2 listening port
```

### 3. Authentication Configuration
Check `/etc/pgpool2/pool_hba.conf`:
```conf
# Example entries
host    all         all         127.0.0.1/32          md5
host    all         all         ::1/128               md5
```

### 4. PostgreSQL pg_hba.conf
Also check PostgreSQL's authentication file `/etc/postgresql/16/main/pg_hba.conf`:
```conf
# Allow pgpool2 to connect
host    all    all    127.0.0.1/32    md5
host    all    all    ::1/128         md5
```

## Commands to Check Configuration

### View pgpool2 config (requires sudo):
```bash
sudo grep -E "^backend_hostname|^backend_port|^backend_weight|^listen_addresses|^port" /etc/pgpool2/pgpool.conf
```

### View pool_hba.conf:
```bash
sudo cat /etc/pgpool2/pool_hba.conf
```

### Check PostgreSQL is accessible:
```bash
psql -h 127.0.0.1 -p 5432 -U postgres -d postgres -c "SELECT version();"
```

### Check pgpool2 logs:
```bash
sudo journalctl -u pgpool2 -n 50 --no-pager
```

### Restart pgpool2 after config changes:
```bash
sudo systemctl restart pgpool2
```

## Quick Fix Commands

### 1. Check if backend is configured correctly:
```bash
sudo sed -n '/^backend_hostname0/p; /^backend_port0/p' /etc/pgpool2/pgpool.conf
```

### 2. Test direct PostgreSQL connection:
```bash
PGPASSWORD=qazwsx psql -h 127.0.0.1 -p 5432 -U postgres -d postgres -c "SELECT 1;"
```

### 3. If backend_hostname0 is wrong, fix it:
```bash
sudo sed -i "s/^backend_hostname0.*/backend_hostname0 = 'localhost'/" /etc/pgpool2/pgpool.conf
sudo systemctl restart pgpool2
```

## Typical Configuration Example

```conf
# /etc/pgpool2/pgpool.conf

listen_addresses = '*'
port = 9999

# Backend 0
backend_hostname0 = 'localhost'
backend_port0 = 5432
backend_weight0 = 1
backend_data_directory0 = '/var/lib/postgresql/16/main'
backend_flag0 = 'ALLOW_TO_FAILOVER'

# Connection pool settings
num_init_children = 32
max_pool = 4
child_life_time = 300
child_max_connections = 0
connection_life_time = 0
client_idle_limit = 0
```

## Troubleshooting Steps

1. **Verify PostgreSQL is running:**
   ```bash
   sudo systemctl status postgresql
   ```

2. **Verify PostgreSQL is listening on port 5432:**
   ```bash
   netstat -tlnp | grep 5432
   ```

3. **Test direct PostgreSQL connection:**
   ```bash
   psql -h 127.0.0.1 -p 5432 -U postgres -d postgres
   ```

4. **Check pgpool2 can reach PostgreSQL:**
   - Ensure `backend_hostname0` matches where PostgreSQL is accessible
   - Ensure `backend_port0` matches PostgreSQL's port (usually 5432)

5. **Check authentication:**
   - Verify `pool_hba.conf` allows your connection
   - Verify PostgreSQL's `pg_hba.conf` allows connections from pgpool2

6. **Restart services:**
   ```bash
   sudo systemctl restart postgresql
   sudo systemctl restart pgpool2
   ```

