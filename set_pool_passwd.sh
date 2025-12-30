#!/bin/bash
# Script to set password in pool_passwd file for pgpool2

echo "=== Setting Password in pool_passwd File ==="
echo ""

echo "This script will help you set the PostgreSQL password in pool_passwd"
echo "so that pgpool2 can authenticate to the PostgreSQL backend."
echo ""

echo "Method 1: Using pg_md5 (Recommended)"
echo "-------------------------------------"
echo "This is the standard way to create/update pool_passwd file."
echo ""
echo "Run this command (it will prompt for the password):"
echo "  sudo pg_md5 -p -u postgres /etc/pgpool2/pool_passwd"
echo ""
echo "Or if you want to specify the password directly:"
echo "  echo 'qazwsx' | sudo pg_md5 -p -u postgres /etc/pgpool2/pool_passwd"
echo ""

echo "Method 2: Manual Creation"
echo "-------------------------"
echo "If pg_md5 is not available, you can create it manually:"
echo ""
echo "1. Generate MD5 hash of password:"
echo "   echo -n 'postgresqazwsx' | md5sum"
echo "   (Note: format is 'username' + 'password' concatenated)"
echo ""
echo "2. Create the file:"
echo "   sudo bash -c \"echo 'postgres:MD5_HASH_HERE' > /etc/pgpool2/pool_passwd\""
echo "   sudo chown postgres:postgres /etc/pgpool2/pool_passwd"
echo "   sudo chmod 600 /etc/pgpool2/pool_passwd"
echo ""

echo "Method 3: Using Python (if pg_md5 not available)"
echo "-------------------------------------------------"
echo "python3 -c \"import hashlib; print('postgres:md5' + hashlib.md5(('postgres' + 'qazwsx').encode()).hexdigest())\""
echo ""

echo "=== Current Status ==="
if [ -f /etc/pgpool2/pool_passwd ]; then
    echo "✓ pool_passwd file exists"
    echo "File contents (first line only):"
    sudo head -1 /etc/pgpool2/pool_passwd 2>/dev/null || echo "  Cannot read file (needs sudo)"
    echo ""
    echo "File permissions:"
    sudo ls -la /etc/pgpool2/pool_passwd 2>/dev/null || echo "  Cannot check permissions"
else
    echo "✗ pool_passwd file does NOT exist"
    echo "  Will be created when you run pg_md5"
fi
echo ""

echo "=== Quick Setup (if password is 'qazwsx') ==="
echo "Run these commands:"
echo ""
echo "  # Set password using pg_md5"
echo "  echo 'qazwsx' | sudo pg_md5 -p -u postgres /etc/pgpool2/pool_passwd"
echo ""
echo "  # Set proper permissions"
echo "  sudo chown postgres:postgres /etc/pgpool2/pool_passwd"
echo "  sudo chmod 600 /etc/pgpool2/pool_passwd"
echo ""
echo "  # Verify the file"
echo "  sudo cat /etc/pgpool2/pool_passwd"
echo "  (Should show: postgres:md5<hash>)"
echo ""

echo "=== After Setting Password ==="
echo "1. Make sure pool_passwd is enabled in pgpool.conf:"
echo "   sudo grep '^pool_passwd' /etc/pgpool2/pgpool.conf"
echo "   (Should show: pool_passwd = '/etc/pgpool2/pool_passwd')"
echo ""
echo "2. Restart pgpool2:"
echo "   sudo systemctl restart pgpool2"
echo ""
echo "3. Check status:"
echo "   sudo systemctl status pgpool2"
echo ""







