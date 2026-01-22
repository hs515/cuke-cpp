# language: en
@ftag
Feature: Data-Table Addition
  In order to avoid silly mistakes
  As a math idiot 
  I want to be told the sum of numbers in a table

  Scenario Outline: Add numbers in a table
    Given I have entered numbers from a table into the calculator
      | number |
      | 2      |
      | 5      |
      | 9      |
    When I press add
    Then the result should be 16 on the screen

  Scenario Outline: Add numbers in a table
    Given I have entered numbers from the below table into the calculator
      | number |
      | 2      |
      | 5      |
      | 9      |
    When I press add
    Then the result should be 16 on the screen
