#!/bin/bash

# Prompt the user to enter the number of operators and operands
while true; do
 echo "Enter the number of operands:"
 read num_operands
 # Check if the input is a number
 if [[ "$num_operands" =~ ^[0-9]+$ ]]; then
   break
 else
   echo "Error: '$num_operands' is not a number."
 fi
done

while true; do
 echo "Enter the number of operators:"
 read num_operators
 # Check if the input is a number
 if [[ "$num_operators" =~ ^[0-9]+$ ]]; then
   break
 else
   echo "Error: '$num_operators' is not a number."
 fi
done

# Check if the number of operands is greater than the number of operators
if (( num_operands <= num_operators )); then
 echo "The number of operands must be greater than the number of operators."
 exit 1
fi

# Check if the number of operators is equal to the number of operands minus one
if (( num_operators != num_operands - 1 )); then
 echo "The number of operators must be one less than the operands."
 exit 1
fi

# Prompt the user to enter all operands
echo "Enter all operands:"
operands=()
for (( i=0; i<$num_operands; i++ )); do
 read operand
 # Check if the input is a number
 if ! [[ "$operand" =~ ^[+-]?[0-9]+\.?[0-9]*$ ]]; then
  echo "Error: '$operand' is not a number."
  exit 1
 fi
 operands+=("$operand")
done

# Prompt the user to enter all operators
echo "Enter all operators:"
operators=()
for (( i=0; i<$num_operators; i++ )); do
 read operator
 operators+=("$operator")
done

# Calculate the result
result=$(echo "scale=100; ${operands[0]}" | bc -l)
for (( i=0; i<$num_operators; i++ )); do
 case ${operators[i]} in
  +)
      result=$(echo "scale=100; $result + ${operands[i+1]}" | bc -l)
      ;;
  -)
      result=$(echo "scale=100; $result - ${operands[i+1]}" | bc -l)
      ;;
  *)
      case ${operators[i]} in
          \*)
             result=$(echo "scale=100; $result * ${operands[i+1]}" | bc -l)
             ;;
          /)
             if (( ${operands[i+1]} == 0 )); then
               echo "Error: Division by zero is not allowed."
               exit 1
             fi
             result=$(echo "scale=100; $result / ${operands[i+1]}" | bc -l)
             ;;
          *)
             echo "Invalid operator: ${operators[i]}"
             exit 1
             ;;
      esac
      ;;
 esac
done

# Output the result
echo "Result: $result"
