#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

// Hardcoded key for decryption (for demonstration purposes)
const char *DECRYPTION_KEY = "combinedsecretkey";

int main() {
    FILE *encrypted_file;
    long file_size;
    unsigned char *encrypted_data;
    unsigned char *decrypted_data;
    char temp_filename[] = "/tmp/combined_decrypted_XXXXXX";
    int fd;
    FILE *temp_exe_file;

    // 1. Read encrypted file
    encrypted_file = fopen("projects/06_packing_unpacking/05_combined_protections/encrypted_stripped_program.enc", "rb");
    if (!encrypted_file) {
        perror("Error opening encrypted file");
        return 1;
    }

    fseek(encrypted_file, 0, SEEK_END);
    file_size = ftell(encrypted_file);
    fseek(encrypted_file, 0, SEEK_SET);

    encrypted_data = (unsigned char *)malloc(file_size);
    if (!encrypted_data) {
        perror("Error allocating memory for encrypted data");
        fclose(encrypted_file);
        return 1;
    }
    fread(encrypted_data, 1, file_size, encrypted_file);
    fclose(encrypted_file);

    // 2. Decrypt data using openssl command (simulated)
    char command[512];
    snprintf(command, sizeof(command), 
             "openssl enc -aes-256-cbc -d -salt -in projects/06_packing_unpacking/05_combined_protections/encrypted_stripped_program.enc -k \"%s\"", 
             DECRYPTION_KEY);

    FILE *pipe = popen(command, "r");
    if (!pipe) {
        perror("Error running openssl command");
        free(encrypted_data);
        return 1;
    }

    decrypted_data = (unsigned char *)malloc(file_size * 2); // Allocate more for safety
    if (!decrypted_data) {
        perror("Error allocating memory for decrypted data");
        pclose(pipe);
        free(encrypted_data);
        return 1;
    }

    long decrypted_size = 0;
    int c;
    while ((c = fgetc(pipe)) != EOF) {
        decrypted_data[decrypted_size++] = (unsigned char)c;
    }
    pclose(pipe);
    free(encrypted_data); // Free encrypted data after use

    // 3. Write decrypted data to a temporary executable file
    fd = mkstemp(temp_filename);
    if (fd == -1) {
        perror("Error creating temporary file");
        free(decrypted_data);
        return 1;
    }
    temp_exe_file = fdopen(fd, "wb");
    if (!temp_exe_file) {
        perror("Error opening temporary file stream");
        close(fd);
        free(decrypted_data);
        return 1;
    }

    fwrite(decrypted_data, 1, decrypted_size, temp_exe_file);
    fclose(temp_exe_file);
    free(decrypted_data); // Free decrypted data after writing

    // 4. Set executable permissions
    if (chmod(temp_filename, S_IRWXU) == -1) { // rwx for owner
        perror("Error setting executable permissions");
        unlink(temp_filename); // Clean up temp file
        return 1;
    }

    // 5. Execute the temporary file
    execl(temp_filename, temp_filename, NULL);

    // If execl returns, it means an error occurred
    perror("Error executing decrypted program");
    unlink(temp_filename); // Clean up temp file
    return 1;
}
