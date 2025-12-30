# Why pgpool2 Service is Not Starting

## Error Message
```
FATAL:  syntax error in configuration file "/etc/pgpool2/pgpool.conf"
DETAIL:  parse error at line 128 'etc/pgpool2/pool_passwd' token = 5
```

## Root Cause

The pgpool2 service is **failing to start** because there is a **syntax error** in the configuration file at **line 128**.

Specifically, the `pool_passwd` configuration line is malformed:
- The path `'etc/pgpool2/pool_passwd'` is **missing the leading slash** `/`
- OR the path is **missing quotes** around it
- OR both issues exist

## What Should It Look Like?

**CORRECT format:**
```conf
pool_passwd = '/etc/pgpool2/pool_passwd'
```

**INCORRECT formats (causing the error):**
```conf
pool_passwd = etc/pgpool2/pool_passwd        # Missing / and quotes
pool_passwd = /etc/pgpool2/pool_passwd       # Missing quotes
pool_passwd = 'etc/pgpool2/pool_passwd'     # Missing leading /
```

## How to Fix

### Step 1: Check the current line
```bash
sudo sed -n '125,135p' /etc/pgpool2/pgpool.conf
```

### Step 2: Fix the syntax
```bash
# Fix the pool_passwd line to have proper quotes and full path
sudo sed -i "s|^pool_passwd =.*|pool_passwd = '/etc/pgpool2/pool_passwd'|" /etc/pgpool2/pgpool.conf
```

### Step 3: Verify the fix
```bash
sudo grep '^pool_passwd' /etc/pgpool2/pgpool.conf
```
Should show: `pool_passwd = '/etc/pgpool2/pool_passwd'`

### Step 4: Test configuration syntax
```bash
sudo pgpool -n -f /etc/pgpool2/pgpool.conf -F /etc/pgpool2/pcp.conf -a /etc/pgpool2/pool_hba.conf 2>&1 | head -20
```
If no errors appear, the syntax is correct.

### Step 5: Restart the service
```bash
sudo systemctl restart pgpool2
```

### Step 6: Verify it's running
```bash
sudo systemctl status pgpool2
```

## Alternative: Temporarily Disable pool_passwd

If you want to test without pool_passwd first:

```bash
# Comment out the problematic line
sudo sed -i 's/^pool_passwd =/#pool_passwd =/' /etc/pgpool2/pgpool.conf

# Restart
sudo systemctl restart pgpool2

# Check status
sudo systemctl status pgpool2
```

**Note:** If you disable pool_passwd, you may encounter authentication errors when connecting, but at least pgpool2 will start and you can verify the basic configuration is working.

## Why This Happened

This error likely occurred when someone tried to enable `pool_passwd` but:
1. Used an incorrect sed command that didn't properly quote the path
2. Manually edited the file and made a typo
3. Copied a configuration example that was incomplete

## Verification After Fix

Once pgpool2 starts successfully:

```bash
# Check service status
sudo systemctl status pgpool2

# Check if port 9999 is listening
sudo netstat -tlnp | grep 9999
# OR
sudo ss -tlnp | grep 9999

# Test connection
psql -h 127.0.0.1 -p 9999 -U postgres -d postgres -c "SELECT version();"
```

## Quick Fix Script

Run the diagnostic script:
```bash
./diagnose_pgpool_failure.sh
```

This will show you the exact error and provide the fix commands.







