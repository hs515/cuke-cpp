# language: en
Feature: Doc-String Addition
  In order to avoid silly mistakes
  As a math idiot 
  I want to be told the sum of numbers in a string

  Scenario Outline: Add numbers in a string
    Given I have entered numbers from a multi-line text into the calculator
      """
        1 2 3 4 5 6 7 8 9 10
      """
    When I press add
    Then the result should be 55 on the screen

  Scenario Outline: Add two numbers
    Given I have entered <input_1> into the calculator
    And I have entered <input_2> into the calculator
    When I press <button>
    Then the result should be <output> on the screen

  Examples:
    | input_1 | input_2 | button | output |
    | 20      | 30      | add    | 40     |
    | 2       | 5       | add    | 7      |
    | 0       | 40      | add    | 40     |
