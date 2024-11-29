import xml.etree.ElementTree as ET
import argparse

def update_sc_prices(file_path):
    # Parse the XML file
    tree = ET.parse(file_path)
    root = tree.getroot()

    # Iterate over all 'sc' elements
    for sc in root.findall('sc'):
        # Get the current price
        price = sc.get('price')
        if price is not None:
            # Convert price to integer, divide by 1000, and update the attribute
            new_price = int(price) / 1000
            sc.set('price', str(int(new_price)))

    # Write the updated XML back to the file
    tree.write(file_path, encoding='utf-8', xml_declaration=True)

def main():
    # Set up argument parser
    parser = argparse.ArgumentParser(description='Update sc prices in an XML file.')
    parser.add_argument('file_path', type=str, help='Path to the XML file')

    # Parse the arguments
    args = parser.parse_args()

    # Update sc prices
    update_sc_prices(args.file_path)

if __name__ == '__main__':
    main()