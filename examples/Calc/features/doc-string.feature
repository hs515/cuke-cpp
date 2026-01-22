# language: en
Feature: Doc-String Addition
  In order to avoid silly mistakes
  As a math idiot 
  I want to be told the sum of numbers in a string

  @stag
  Scenario Outline: Add numbers in a string
    Given I have entered numbers from a multi-line text into the calculator
      """
        1 2 3 4 5 6 7 8 9 10
      """
    When I press add
    Then the result should be 55 on the screen

  Scenario Outline: Add numbers in a string
    Given I have entered numbers from the below multi-line text into the calculator
      """
        1 2 3 4 5 6 7 8 9 10
      """
    When I press add
    Then the result should be 55 on the screen
