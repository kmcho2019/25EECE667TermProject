Encrypting with zip

This is the most straightforward method, as the encryption is a built-in feature. It will prompt you to set a password.

To Encrypt a File or Folder:

Use the -e flag (for encrypt) and -r (to recursively include all files in a folder).

```bash

# Encrypt hidden design files

zip -e hidden_designs.zip  test12.input.def  test12.input.lef  test13.input.def  test13.input.lef 

```

After running this command, you will be prompted to enter and verify a password.

To Decrypt/Unzip:

Use the standard unzip command. It will automatically detect the encryption and prompt you for the password.

```bash

unzip hidden_designs.zip

```
