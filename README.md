# CGI Query Parser

A simple CGI (Common Gateway Interface) program to parse and display query strings from client requests in a structured HTML format.

## Features

- Parse query strings with multiple key-value pairs.
- Convert query data into an HTML unordered list.
- Handle and display query strings in a formatted manner on the web page.
- Demonstrate a CGI program that forcefully terminates itself (for testing purposes).

## How It Works

1. **Parsing Query Strings**: Extracts `name=value` pairs from a provided query string and returns an array of structured data.

2. **Displaying as HTML**: Converts an array of name-value pairs into an HTML unordered list.

3. **Termination Demo**: A CGI program that starts displaying an HTML page but terminates itself mid-execution for demonstration purposes.

## Setup

1. Ensure you have a C compiler (like gcc) installed.
2. Compile the C files:
    ```
    gcc <filename>.c -o output_name
    ```
3. Configure your server to use the compiled CGI programs.

## Usage

1. Run your server.
2. Make a request with a query string (e.g., `http://yourserver.com/cgi-bin/output_name?name1=value1&name2=value2`).
3. View the parsed query string displayed as an HTML list on the page.

## Contribute

Contributions are welcome! Please submit a pull request or create an issue if you want to add new features or find any bugs.

## License

MIT


